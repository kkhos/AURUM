
#include "AccessibleWatcher.h"

#include <string.h>
#include <iostream>
#include <utility>

#include "loguru.hpp"

AtspiEventListener *AccessibleWatcher::listener = nullptr;

static bool iShowingNode(AtspiAccessible *node)
{
    char *name = atspi_accessible_get_name(node, NULL);
    char *pname = atspi_accessible_get_name(
        atspi_accessible_get_parent(node, NULL), NULL);

    LOG_SCOPE_F(INFO, "isShowing %s %s", name, pname);

    // if (!strcmp(name, "Keyboard") && !strcmp(pname, "ise-default")) {
    //     free(name);
    //     free(pname);
    //     return false;
    // }
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

AccessibleWatcher::AccessibleWatcher() : mActivatedWindowList{}, mWindowSet{}
{
    GVariant *enabled_variant = nullptr, *result = nullptr;
    GError *  error = nullptr;
    atspi_init();
    atspi_set_main_context (g_main_context_default ());

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
        atspi_event_listener_new(AccessibleWatcher::onAtspiWindowEvent, this, NULL);
    atspi_event_listener_register(listener, "window:create", NULL);
    atspi_event_listener_register(listener, "window:destroy", NULL);
    atspi_event_listener_register(listener, "window:activate", NULL);
    atspi_event_listener_register(listener, "window:deactivate", NULL);
    atspi_event_listener_register(listener, "object:", NULL);
}

AccessibleWatcher::~AccessibleWatcher()
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

AccessibleNode *AccessibleWatcher::getRootNode() const
{
    AtspiAccessible *node = atspi_get_desktop(0);
    AccessibleNode * accNode = AccessibleNode::get(node);
    if (node) g_object_unref(node);
    return accNode;
}

AccessibleNode *AccessibleWatcher::getTopNode() const
{
    AtspiAccessible *topNode = nullptr, *activeNode = nullptr,
                    *rootNode = nullptr;
    {
        std::unique_lock<std::mutex> lock(mLock);
        if (!mActivatedWindowList.empty()) {
            topNode = mActivatedWindowList.front();
            if (topNode && iShowingNode(topNode))
                return AccessibleNode::get(atspi_accessible_get_application(topNode, NULL));
        }
    }

    LOG_F(INFO, "Mo activated window node or Invisible acticated window / topNdoe(%p)", topNode);
    LOG_F(INFO, "Trying fallback logic");

    rootNode = atspi_get_desktop(0);

    if (rootNode) {
        activeNode = atspi_accessible_get_application(findActiveNode(rootNode, 0, 2), NULL);
        if (activeNode) {
            AccessibleNode *node = AccessibleNode::get(activeNode);
            return node;
        }
        AccessibleNode *node = AccessibleNode::get(rootNode);
        return node;
    }
    return nullptr;
}

void AccessibleWatcher::onAtspiWindowEvent(AtspiEvent *event, void *user_data)
{
    char *              name, *pname;
    IAtspiEvents *instance = (IAtspiEvents *)user_data;

    AtspiAccessible *p = atspi_accessible_get_parent(event->source, NULL);

    name = atspi_accessible_get_name(event->source, NULL);
    pname = atspi_accessible_get_name(p, NULL);

    // LOG_SCOPE_F(INFO, "event:%s, src:%p(%s), p:%p(%s), d1:%p d2:%p instance:%p",
    //             event->type, event->source, name, p, pname, event->detail1,
    //             event->detail2, instance);

    if (!strcmp(event->type, "window:activate")) {
        instance->onWindowActivated(
            static_cast<AtspiAccessible *>(event->source),
            static_cast<WindowActivateInfoType>(event->detail1));
    } else if (!strcmp(event->type, "window:deactivate")) {
        instance->onWindowDeactivated(static_cast<AtspiAccessible *>(event->source));
    } else if (!strcmp(event->type, "window:create")) {
        instance->onWindowCreated(static_cast<AtspiAccessible *>(event->source));
    } else if (!strcmp(event->type, "window:destroy")) {
        instance->onWindowDestroyed(static_cast<AtspiAccessible *>(event->source));
    } else if (!strcmp(event->type, "object:state-changed:visible")) {
        instance->onVisibilityChanged(
            static_cast<AtspiAccessible *>(event->source),
            (event->detail1 != 0));
    } else if (!strcmp(event->type, "object:state-changed:defunct")) {
        instance->onObjectDefunct(
            static_cast<AtspiAccessible *>(event->source));
    }
}

void AccessibleWatcher::printDbgInformation() const
{
    LOG_SCOPE_F(INFO, "%d %d", mActivatedWindowList.size(),  mWindowSet.size());

    for (auto iter = mActivatedWindowList.begin(); iter != mActivatedWindowList.end(); ++iter) {
        LOG_F(INFO, "%p", *iter);
    }
        LOG_F(INFO, "-----------");

    for (auto iter = mWindowSet.begin(); iter != mWindowSet.end(); ++iter) {
        LOG_F(INFO, "%p", *iter);
    }
}

const AccessibleWatcher *AccessibleWatcher::getInstance()
{
    static AccessibleWatcher *mInstance = nullptr;
    if (!mInstance) mInstance = new AccessibleWatcher();
    return mInstance;
}

void AccessibleWatcher::clearWindowList() const
{
    std::unique_lock<std::mutex> lock(mLock);
    while (!mActivatedWindowList.empty()) {
        AtspiAccessible *n = mActivatedWindowList.front();
        mActivatedWindowList.pop_front();
        g_object_unref(n);
    }

    mWindowSet.clear();
}

bool AccessibleWatcher::removeFromActivatedList(AtspiAccessible *node)
{
    mActivatedWindowList.remove_if([&](auto &n) { return n == node; });
    return true;
}

bool AccessibleWatcher::addToActivatedList(AtspiAccessible *node)
{
    mActivatedWindowList.remove_if([&](auto &n) { return n == node; });
    //if (!strcmp(name, "Keyboard") && !strcmp(pname, "ise-default")) return;
    mActivatedWindowList.push_front(node);

    auto iter = mWindowSet.find(node);
    if ( iter == mWindowSet.end()) mWindowSet.insert(node);
    return true;
}

bool AccessibleWatcher::removeFromWindowSet(AtspiAccessible *node)
{
    removeFromActivatedList(node);
    auto iter = mWindowSet.find(node);
    if ( iter != mWindowSet.end()){
        mWindowSet.erase(node);
        return true;
    }
    return false;
}

bool AccessibleWatcher::addToWindowSet(AtspiAccessible *node)
{
    auto iter = mWindowSet.find(node);
    if ( iter == mWindowSet.end()){
        mWindowSet.insert(node);
        return true;
    }
    return false;
}

void AccessibleWatcher::onWindowActivated(AtspiAccessible *      node,
                            WindowActivateInfoType type)
{
    std::unique_lock<std::mutex> lock(mLock);

    addToActivatedList(node);

    return;
    /*
    char *name = atspi_accessible_get_name(node, NULL);
    char *pname = atspi_accessible_get_name(
        atspi_accessible_get_parent(node, NULL), NULL);
    LOG_SCOPE_F(INFO, "%s %s", name, pname);

    mActivatedWindowList.remove_if([&](auto &n) { return n == node; });
    if (!strcmp(name, "Keyboard") && !strcmp(pname, "ise-default")) return;
    mActivatedWindowList.push_front(node);
    */
}

void AccessibleWatcher::onWindowDeactivated(AtspiAccessible *node)
{
    std::unique_lock<std::mutex> lock(mLock);
    removeFromActivatedList(node);
    return;
/*
    mActivatedWindowList.remove_if([&](auto &n) { return n == node; });
    mActivatedWindowList.push_back(node);
    */
}

void AccessibleWatcher::onWindowCreated(AtspiAccessible *node)
{
    std::unique_lock<std::mutex> lock(mLock);
    addToWindowSet(node);
}

void AccessibleWatcher::onWindowDestroyed(AtspiAccessible *node)
{
    std::unique_lock<std::mutex> lock(mLock);
    removeFromWindowSet(node);
}

void AccessibleWatcher::onVisibilityChanged(AtspiAccessible *node, bool visible)
{
}

void AccessibleWatcher::onObjectDefunct(AtspiAccessible *node)
{
    LOG_SCOPE_F(INFO, "object defuncted %p", node);
}