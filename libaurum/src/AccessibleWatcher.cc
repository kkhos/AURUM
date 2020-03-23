
#include "AccessibleWatcher.h"

#include <string.h>
#include <iostream>
#include <utility>

#include "loguru.hpp"

AtspiEventListener *AccessibleWatcher::listener = nullptr;



static bool iShowingNode(AtspiAccessible *node)
{
    char *name = NULL;
    if (node) name = atspi_accessible_get_name(node, NULL);
    else return false;

    LOG_SCOPE_F(INFO, "isShowing %s", name);
    auto stateSet = make_gobj_unique(atspi_accessible_get_state_set(node));
    auto states = make_garray_unique(atspi_state_set_get_states(stateSet.get()));

    if (atspi_state_set_contains(stateSet.get(), ATSPI_STATE_ACTIVE)
        && atspi_state_set_contains(stateSet.get(), ATSPI_STATE_SHOWING)) {
        LOG_F(INFO, "active and showing %p %s", node, name);
        free(name);
        return true;
    }
    free(name);
    return false;
}

static std::vector<AtspiAccessible *>
findActiveNode(AtspiAccessible *node, int depth,
                                       int max_depth)
{
    std::vector<AtspiAccessible *> ret{};
    if (depth >= max_depth) return ret;

    if (iShowingNode(node)) {
        g_object_ref(node);
        char *name = atspi_accessible_get_name(node, NULL);
        LOG_SCOPE_F(INFO, "%s", name);
        ret.push_back(node);
        return ret;
    }

    int nchild = atspi_accessible_get_child_count(node, NULL);
    for (int i = 0; i < nchild; i++) {
        auto child = make_gobj_unique(atspi_accessible_get_child_at_index(node, i, NULL));
        std::vector<AtspiAccessible *> childRet = findActiveNode(child.get(), depth + 1, max_depth);
        ret.insert(ret.end(), childRet.begin(), childRet.end());
    }

    return ret;
}

AccessibleWatcher::AccessibleWatcher() : mActivatedWindowList{}, mWindowSet{}
{
    GVariant *enabled_variant = nullptr, *result = nullptr;
    GError *  error = nullptr;
    atspi_set_main_context (g_main_context_default ());
    atspi_init();

    listener =
        atspi_event_listener_new(AccessibleWatcher::onAtspiWindowEvent, this, NULL);
    atspi_event_listener_register(listener, "window:create", NULL);
    atspi_event_listener_register(listener, "window:destroy", NULL);
    atspi_event_listener_register(listener, "window:activate", NULL);
    atspi_event_listener_register(listener, "window:deactivate", NULL);
    atspi_event_listener_register(listener, "object:", NULL);

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

}

AccessibleWatcher::~AccessibleWatcher()
{
    GVariant *enabled_variant = nullptr, *result = nullptr;
    GError *  error = nullptr;

    enabled_variant = g_variant_new_boolean(false);
    result = g_dbus_proxy_call_sync(
        mDbusProxy, "Set",
        g_variant_new("(ssv)", "org.a11y.Status", "IsEnabled", enabled_variant),
        G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);


    for (auto it = mAccessibleNode.begin(); it != mAccessibleNode.end(); ++it) {
        g_object_unref(it->first);
        delete it->second;
    }
    atspi_event_listener_deregister(listener, "window:", NULL);
    atspi_event_listener_deregister(listener, "object:", NULL);

    g_object_unref(listener);
    g_object_unref(mDbusProxy);
    g_variant_unref(enabled_variant);
    g_variant_unref(result);

    atspi_event_quit();
    atspi_exit();
}

std::unique_ptr<AccessibleNode> AccessibleWatcher::getRootNode() const
{
    auto node = make_gobj_unique(atspi_get_desktop(0));
    auto accNode = AccessibleNode::get(node.get());
    if(node.get()) g_object_unref(node.get());
    return accNode;
}

std::vector<std::unique_ptr<AccessibleNode>> AccessibleWatcher::getTopNode() const
{
    AtspiAccessible *topNode = nullptr, *activeNode = nullptr;
    std::vector<std::unique_ptr<AccessibleNode>> ret;

    {
        std::unique_lock<std::mutex> lock(mLock);
        if (!mActivatedWindowList.empty()) {
            topNode = mActivatedWindowList.front();
            std::list<AtspiAccessible *>::const_iterator iterator;
            for (iterator = mActivatedWindowList.begin(); iterator != mActivatedWindowList.end(); ++iterator){
                if (*iterator && iShowingNode(*iterator)) {
                    AtspiAccessible *child = atspi_accessible_get_application(*iterator, NULL);
                    if (child) {
                        auto tmpNode = make_gobj_unique(child);
                        auto node =  AccessibleNode::get(tmpNode.get());
                        if (tmpNode.get()) g_object_unref(tmpNode.get());
                        ret.push_back(std::move(node));
                    }
                }
            }
            return ret;
        }
    }

    LOG_F(INFO, "Mo activated window node or Invisible acticated window / topNdoe(%p)", topNode);
    LOG_F(INFO, "Trying fallback logic");

    auto rootNode = make_gobj_unique(atspi_get_desktop(0));

    if (rootNode) {
        std::vector<AtspiAccessible*> activeNodes = findActiveNode(rootNode.get(), 0, 2);
        if (!activeNodes.empty()) {
            std::vector<AtspiAccessible*>::const_iterator iterator;
            for (iterator = activeNodes.begin(); iterator != activeNodes.end(); ++iterator){
                auto tmpNode = make_gobj_unique(atspi_accessible_get_application(*iterator, NULL));
                auto node = AccessibleNode::get(tmpNode.get());
                if (tmpNode.get()) g_object_unref(tmpNode.get());
                g_object_unref(*iterator);
                ret.push_back(std::move(node));
            }
        } else {
            auto node = AccessibleNode::get(rootNode.get());
            if (rootNode.get()) g_object_unref(rootNode.get());
            ret.push_back(std::move(node));
        }
    }
    return ret;
}

void AccessibleWatcher::onAtspiWindowEvent(AtspiEvent *event, void *user_data)
{
    char *name = NULL, *pname = NULL;
    IAtspiEvents *instance = (IAtspiEvents *)user_data;

    if (!event->source)
    {
        LOG_F(INFO, "event->source is NULL. Skip event handling");
        return;
    }

    auto p = make_gobj_unique(atspi_accessible_get_parent(event->source, NULL));

    name = atspi_accessible_get_name(event->source, NULL);
    if (p) pname = atspi_accessible_get_name(p.get(), NULL);

     LOG_SCOPE_F(INFO, "event:%s, src:%p(%s), p:%p(%s), d1:%p d2:%p instance:%p",
                 event->type, event->source, name, p.get(), pname, event->detail1,
                 event->detail2, instance);

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
    if (name) free(name);
    if (pname) free(pname);
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
}

void AccessibleWatcher::onWindowDeactivated(AtspiAccessible *node)
{
    std::unique_lock<std::mutex> lock(mLock);
    removeFromActivatedList(node);
    return;
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