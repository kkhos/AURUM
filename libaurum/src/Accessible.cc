
#include "Accessible.h"

#include <string.h>
#include <iostream>
#include <utility>

#include "loguru.hpp"

AtspiEventListener *Accessible::listener = nullptr;

static bool iShowingNode(AtspiAccessible *node)
{
    char *name = atspi_accessible_get_name(node, NULL);
    char *pname = atspi_accessible_get_name(
        atspi_accessible_get_parent(node, NULL), NULL);

    LOG_SCOPE_F(INFO, "isShowing %s %s", name, pname);

    if (!strcmp(name, "Keyboard") && !strcmp(pname, "ise-default")) {
        free(name);
        free(pname);
        return false;
    }
    free(name);
    free(pname);

    AtspiStateSet *stateSet = atspi_accessible_get_state_set(node);
    if (atspi_state_set_contains(stateSet, ATSPI_STATE_ACTIVE) &&
        atspi_state_set_contains(stateSet, ATSPI_STATE_SHOWING)) {
        g_object_unref(stateSet);
        return true;
    }
    return false;
}

static AtspiAccessible *findActiveNode(AtspiAccessible *node, int depth,
                                       int max_depth)
{
    if (depth >= max_depth) return NULL;

    if (iShowingNode(node)) {
        g_object_ref(node);

        char *name = atspi_accessible_get_name(node, NULL);
        char *pname = atspi_accessible_get_name(
            atspi_accessible_get_parent(node, NULL), NULL);
        LOG_SCOPE_F(INFO, "%s %s", name, pname);
        return node;
    }

    int nchild = atspi_accessible_get_child_count(node, NULL);
    for (int i = 0; i < nchild; i++) {
        AtspiAccessible *child =
            atspi_accessible_get_child_at_index(node, i, NULL);
        AtspiAccessible *active = findActiveNode(child, depth + 1, max_depth);
        g_object_unref(child);
        if (active) return active;
    }

    return NULL;
}

Accessible::Accessible() : mWindowList{}
{
    GVariant *enabled_variant = nullptr, *result = nullptr;
    GError *  error = nullptr;
    atspi_init();

    mDbusProxy = g_dbus_proxy_new_for_bus_sync(
        G_BUS_TYPE_SESSION, G_DBUS_PROXY_FLAGS_NONE,
        NULL, /* GDBusInterfaceInfo */
        "org.a11y.Bus", "/org/a11y/bus", "org.freedesktop.DBus.Properties",
        NULL, &error);

    enabled_variant = g_variant_new_boolean(true);
    result = g_dbus_proxy_call_sync(
        mDbusProxy, "Set",
        g_variant_new("(ssv)", "org.a11y.Status", "IsEnabled", enabled_variant),
        G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);

    g_variant_unref(enabled_variant);
    g_variant_unref(result);

    listener =
        atspi_event_listener_new(Accessible::onAtspiWindowEvent, this, NULL);
    atspi_event_listener_register(listener, "window:", NULL);
    atspi_event_listener_register(listener, "object:", NULL);
}

Accessible::~Accessible()
{
    for (auto it = mAccessibleNode.begin(); it != mAccessibleNode.end(); ++it) {
        g_object_unref(it->first);
        delete it->second;
    }
    atspi_event_listener_deregister(listener, "window:", NULL);
    atspi_event_listener_deregister(listener, "object:", NULL);

    g_object_unref(listener);
    g_object_unref(mDbusProxy);

    atspi_event_quit();
    atspi_exit();
}

AccessibleNode *Accessible::getRootNode() const
{
    AtspiAccessible *node = atspi_get_desktop(0);
    AccessibleNode * accNode = AccessibleNode::get(node);
    if (node) g_object_unref(node);
    return accNode;
}

AccessibleNode *Accessible::getTopNode() const
{
    AtspiAccessible *topNode = nullptr, *activeNode = nullptr,
                    *rootNode = nullptr;
    {
        std::unique_lock<std::mutex> lock(mLock);
        if (!mWindowList.empty()) {
            topNode = mWindowList.front();
        }
    }

#ifdef TIZEN
    char *name, *pname;
    name = atspi_accessible_get_name(topNode, NULL);
    pname = atspi_accessible_get_parent(topNode, NULL)
                ? atspi_accessible_get_name(
                      atspi_accessible_get_parent(topNode, NULL), NULL)
                : NULL;

    LOG_F(INFO, "topNode unique id: %s  /  name: %s pname :%s",
          atspi_accessible_get_unique_id(topNode, NULL), name, pname);

    free(name);
    free(pname);
#endif

    if (topNode) {
        if (iShowingNode(topNode)) {
            AccessibleNode *node = AccessibleNode::get(topNode);
            // g_object_unref(activeNode);
            return node;
        }
    }

    rootNode = atspi_get_desktop(0);

    if (rootNode) {
        activeNode = findActiveNode(rootNode, 0, 2);

#ifdef TIZEN
        char *name, *pname;
        name = atspi_accessible_get_name(activeNode, NULL);
        pname = atspi_accessible_get_parent(activeNode, NULL)
                    ? atspi_accessible_get_name(
                          atspi_accessible_get_parent(activeNode, NULL), NULL)
                    : NULL;

        LOG_F(INFO, "activeNode unique id: %s  /   name: %s pname:%s",
              atspi_accessible_get_unique_id(activeNode, NULL), name, pname);
        free(name);
        free(pname);
#endif

        if (activeNode) {
            AccessibleNode *node = AccessibleNode::get(activeNode);
            return node;
        }
        AccessibleNode *node = AccessibleNode::get(rootNode);
        return node;
    }
    return nullptr;
}

void Accessible::onAtspiWindowEvent(AtspiEvent *event, void *user_data)
{
    char *              name, *pname;
    const IAtspiEvents *instance = (IAtspiEvents *)user_data;

    AtspiAccessible *p = atspi_accessible_get_parent(event->source, NULL);

    name = atspi_accessible_get_name(event->source, NULL);
    pname = atspi_accessible_get_name(p, NULL);

    LOG_SCOPE_F(INFO, "event:%s, src:%p(%s), p:%p(%s), d1:%p d2:%p instance:%p",
                event->type, event->source, name, p, pname, event->detail1,
                event->detail2, instance);

    if (!strcmp(event->type, "window:activate")) {
        instance->onActivate(
            static_cast<AtspiAccessible *>(event->source),
            static_cast<WindowActivateInfoType>(event->detail1));
    } else if (!strcmp(event->type, "window:deactivate")) {
        instance->onDeactivate(static_cast<AtspiAccessible *>(event->source));
    } else if (!strcmp(event->type, "object:state-changed:visible")) {
        instance->onVisibilityChanged(
            static_cast<AtspiAccessible *>(event->source),
            (event->detail1 != 0));
    } else if (!strcmp(event->type, "object:state-changed:defunct")) {
        instance->onObjectDefunct(
            static_cast<AtspiAccessible *>(event->source));
    }
}

const Accessible *Accessible::getInstance()
{
    static Accessible *mInstance = nullptr;
    if (!mInstance) mInstance = new Accessible();
    return mInstance;
}

void Accessible::clearWindowList() const
{
    std::unique_lock<std::mutex> lock(mLock);
    while (!mWindowList.empty()) {
        AtspiAccessible *n = mWindowList.front();
        mWindowList.pop_front();
        g_object_unref(n);
    }
}

void Accessible::onActivate(AtspiAccessible *      node,
                            WindowActivateInfoType type) const
{
    std::unique_lock<std::mutex> lock(mLock);

    char *name = atspi_accessible_get_name(node, NULL);
    char *pname = atspi_accessible_get_name(
        atspi_accessible_get_parent(node, NULL), NULL);
    LOG_SCOPE_F(INFO, "%s %s", name, pname);

    mWindowList.remove_if([&](auto &n) { return n == node; });
    if (!strcmp(name, "Keyboard") && !strcmp(pname, "ise-default")) return;
    mWindowList.push_front(node);
}

void Accessible::onDeactivate(AtspiAccessible *node) const
{
    std::unique_lock<std::mutex> lock(mLock);
    mWindowList.remove_if([&](auto &n) { return n == node; });
    mWindowList.push_back(node);
}

void Accessible::onVisibilityChanged(AtspiAccessible *node, bool visible) const
{
}

void Accessible::onObjectDefunct(AtspiAccessible *node) const
{
    LOG_SCOPE_F(INFO, "object defuncted %p", node);
}