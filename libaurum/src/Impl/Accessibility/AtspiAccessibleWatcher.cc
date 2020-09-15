#include "AtspiAccessibleWatcher.h"

#include "AtspiAccessibleApplication.h"
#include "AtspiAccessibleWindow.h"
#include "AtspiAccessibleNode.h"

#include <algorithm>

#include <loguru.hpp>

AtspiEventListener *AtspiAccessibleWatcher::listener = nullptr;

static bool iShowingNode(AtspiAccessible *node)
{
    char *name = NULL;
    if (node) name = atspi_accessible_get_name(node, NULL);
    else return false;

    LOG_SCOPE_F(INFO, "isShowing %s", name);
    auto stateSet = atspi_accessible_get_state_set(node);

    if (atspi_state_set_contains(stateSet, ATSPI_STATE_ACTIVE)
        && atspi_state_set_contains(stateSet, ATSPI_STATE_SHOWING)) {
        LOG_F(INFO, "active and showing %p %s", node, name);
        free(name);
        g_object_unref(stateSet);
        return true;
    }
    free(name);
    g_object_unref(stateSet);
    return false;
}

static std::vector<AtspiAccessible *>
findActiveNode(AtspiAccessible *node, int depth,
                                       int max_depth)
{
    LOG_SCOPE_F(INFO, "findActiveNode %p %d/%d", node, depth, max_depth);

    std::vector<AtspiAccessible *> ret{};

    if (iShowingNode(node)) {
        g_object_ref(node);
        char *name = atspi_accessible_get_name(node, NULL);
        if (name) {
            LOG_SCOPE_F(INFO, "%s", name);
            free(name);
        }
        ret.push_back(node);
        return ret;
    }

    if (depth >= max_depth) return ret;

    int nchild = atspi_accessible_get_child_count(node, NULL);
    if (nchild <= 0) return ret;

    LOG_F(INFO, "findActiveNode node %p has %d children", node, nchild);
    for (int i = 0; i < nchild; i++) {
        AtspiAccessible* child = atspi_accessible_get_child_at_index(node, i, NULL);
        LOG_F(INFO, "a child found @ %d : %p", i, child);
        std::vector<AtspiAccessible *> childRet = findActiveNode(child, depth + 1, max_depth);
        ret.insert(ret.end(), childRet.begin(), childRet.end());
        g_object_unref(child);
    }

    return ret;
}

AtspiAccessibleWatcher::AtspiAccessibleWatcher()
: mDbusProxy{nullptr}, mLock{}
{
    GVariant *enabled_variant = nullptr, *result = nullptr;
    GError *  error = nullptr;
    atspi_set_main_context (g_main_context_default ());
    atspi_init();

    listener =
        atspi_event_listener_new(AtspiAccessibleWatcher::onAtspiWindowEvent, this, NULL);

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

AtspiAccessibleWatcher::~AtspiAccessibleWatcher()
{
    GVariant *enabled_variant = nullptr, *result = nullptr;
    GError *  error = nullptr;

    enabled_variant = g_variant_new_boolean(false);
    result = g_dbus_proxy_call_sync(
        mDbusProxy, "Set",
        g_variant_new("(ssv)", "org.a11y.Status", "IsEnabled", enabled_variant),
        G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);

    atspi_event_listener_deregister(listener, "window:", NULL);
    atspi_event_listener_deregister(listener, "object:", NULL);

    g_object_unref(listener);
    g_object_unref(mDbusProxy);
    g_variant_unref(enabled_variant);
    g_variant_unref(result);

    atspi_event_quit();
    atspi_exit();
}


void AtspiAccessibleWatcher::onAtspiWindowEvent(AtspiEvent *event, void *user_data)
{
    char *name = NULL, *pname = NULL;
    AtspiAccessibleWatcher *instance = (AtspiAccessibleWatcher *)user_data;

    if (!event->source)
    {
        LOG_F(INFO, "event->source is NULL. Skip event handling");
        return;
    }


    AtspiAccessible *parent = atspi_accessible_get_parent(event->source, NULL);

    name = atspi_accessible_get_name(event->source, NULL);
    if (parent) {
        pname = atspi_accessible_get_name(parent, NULL);
        g_object_unref(parent);
    }

     LOG_SCOPE_F(INFO, "event:%s, src:%p(%s), p:%p(%s), d1:%p d2:%p instance:%p",
                 event->type, event->source, name, parent, pname, event->detail1,
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

//    instance->print_debug();
}

void AtspiAccessibleWatcher::print_debug()
{
    LOG_F(INFO, "activatewindowlist-------------------");
    std::for_each(mActivatedWindowList.begin(), mActivatedWindowList.end(), [](auto acc){
        LOG_F(INFO, "child:%p", acc);
    });

    LOG_F(INFO, "mActivatedApplicationList--------------------------");
    std::for_each(mActivatedApplicationList.begin(), mActivatedApplicationList.end(), [](auto acc){
        LOG_F(INFO, "child:%p", acc);
    });

    LOG_F(INFO, "mWindowSet------------------------------");
    std::for_each(mWindowSet.begin(), mWindowSet.end(), [](auto acc){
        LOG_F(INFO, "child:%p", acc);
    });
    LOG_F(INFO, "------------------------------");
}

void AtspiAccessibleWatcher::onWindowActivated(AtspiAccessible *node,
                            WindowActivateInfoType type)
{
    //std::unique_lock<std::mutex> lock(mLock);
    LOG_SCOPE_F(INFO, "onWindowActivated obj:%p", node);
    //addToActivatedList((node));
    return;
}

void AtspiAccessibleWatcher::onWindowDeactivated(AtspiAccessible *node)
{
    std::unique_lock<std::mutex> lock(mLock);
    LOG_SCOPE_F(INFO, "onWindowDeactivated obj:%p", node);
    //removeFromActivatedList(node);
}

void AtspiAccessibleWatcher::onWindowCreated(AtspiAccessible *node)
{
    std::unique_lock<std::mutex> lock(mLock);
    LOG_SCOPE_F(INFO, "onWindowCreated obj:%p", node);
    //addToWindowSet(node);
}

void AtspiAccessibleWatcher::onWindowDestroyed(AtspiAccessible *node)
{
    std::unique_lock<std::mutex> lock(mLock);
    LOG_SCOPE_F(INFO, "onWindowDestroyed obj:%p vis:%d", node);
    //removeFromWindowSet(node);
}

void AtspiAccessibleWatcher::onVisibilityChanged(AtspiAccessible *node, bool visible)
{
    std::unique_lock<std::mutex> lock(mLock);
    LOG_SCOPE_F(INFO, "onVisibilityChanged obj:%p vis:%d", node, visible);
}

void AtspiAccessibleWatcher::onObjectDefunct(AtspiAccessible *node)
{
    std::unique_lock<std::mutex> lock(mLock);
    LOG_SCOPE_F(INFO, "onObjectDefunct obj:%p", node);
}


int AtspiAccessibleWatcher::getApplicationCount(void) const
{
    AtspiAccessible *root = atspi_get_desktop(0);
    int nchild = atspi_accessible_get_child_count(root, NULL);
    g_object_unref(root);
    if (nchild <= 0) return 0;
    return nchild;
}

std::shared_ptr<AccessibleApplication> AtspiAccessibleWatcher::getApplicationAt(int index) const
{
    AtspiAccessible *root = atspi_get_desktop(0);
    AtspiAccessible* child = atspi_accessible_get_child_at_index(root, index, NULL);
    g_object_unref(root);
    return std::make_shared<AtspiAccessibleApplication>(std::make_shared<AtspiAccessibleNode>(child));
}

std::vector<std::shared_ptr<AccessibleApplication>> AtspiAccessibleWatcher::getApplications(void) const
{
    LOG_SCOPE_F(INFO, "getApplications for this(%p)", this);
    std::vector<std::shared_ptr<AccessibleApplication>> ret{};
    AtspiAccessible *root = atspi_get_desktop(0);
    int nchild = atspi_accessible_get_child_count(root, NULL);
    if (nchild <= 0) {
        g_object_unref(root);
        return ret;
    }

    for (int i = 0; i < nchild; i++){
        AtspiAccessible* child = atspi_accessible_get_child_at_index(root, i, NULL);
        if (child) {
            ret.push_back(std::make_shared<AtspiAccessibleApplication>(std::make_shared<AtspiAccessibleNode>(child)));
        }
    }
    g_object_unref(root);
    return ret;
}

bool AtspiAccessibleWatcher::removeFromActivatedList(AtspiAccessible *node)
{
    LOG_SCOPE_F(INFO,"remove from activelist node %p", node);
    mActivatedWindowList.remove_if([&](auto &n) { return n == node; });

    AtspiAccessible *app = atspi_accessible_get_application(node, NULL);
    LOG_F(INFO, "node:%p, app:%p", node, app);
    if (app) {
        mActivatedApplicationList.remove_if([&](auto &n) { return n == app; });
        g_object_unref(app);
    }
    return true;
}

bool AtspiAccessibleWatcher::addToActivatedList(AtspiAccessible *node)
{
    LOG_SCOPE_F(INFO,"add to activelist node %p", node);
    mActivatedWindowList.remove_if([&](auto &n) { return n == node; });
    mActivatedWindowList.push_front(node);

    auto iter = mWindowSet.find(node);
    if ( iter == mWindowSet.end()) mWindowSet.insert(node);

    AtspiAccessible *app = atspi_accessible_get_application(node, NULL);
    LOG_F(INFO, "node:%p, app:%p", node, app);
    if (app) {
        mActivatedApplicationList.remove_if([&](auto &n) { if(n == app) { g_object_unref(app); return true;} else return false; });
        mActivatedApplicationList.push_front(app);
    }

    return true;
}

bool AtspiAccessibleWatcher::removeFromWindowSet(AtspiAccessible *node)
{
    removeFromActivatedList(node);
    auto iter = mWindowSet.find(node);
    if ( iter != mWindowSet.end()){
        mWindowSet.erase(node);
        return true;
    }
    return false;
}

bool AtspiAccessibleWatcher::addToWindowSet(AtspiAccessible *node)
{
    auto iter = mWindowSet.find(node);
    if ( iter == mWindowSet.end()){
        mWindowSet.insert(node);
        return true;
    }
    return false;
}

/*
std::shared_ptr<AccessibleNode> AtspiAccessibleWatcher::getRootNode() const
{
    auto node = atspi_get_desktop(0);
    auto aNode = std::make_shared<AccessibleNode>(node);
    return accNode;
}

std::vector<std::shared_ptr<AccessibleNode>> AtspiAccessibleWatcher::getTopNode() const
{
    AtspiAccessible *topNode = nullptr, *activeNode = nullptr;
    std::vector<std::shared_ptr<AccessibleNode>> ret;

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
                        ret.push_back((node));
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
            std::set<AtspiAccessible*> appset{};
            for (auto iterator = activeNodes.begin(); iterator != activeNodes.end(); ++iterator){
                //auto tmpNode = make_gobj_unique(atspi_accessible_get_application(*iterator, NULL));
                auto app = atspi_accessible_get_application(*iterator, NULL);
                appset.insert(app);
            }
            for (auto iterator = appset.begin(); iterator != appset.end(); ++iterator){
                auto node = AccessibleNode::get(*iterator);
                g_object_unref(*iterator);
                LOG_F(INFO, "app has showing window found %p %s %s", node.get(), node->getText().c_str(), node->getPkg().c_str());
                ret.push_back((node));
            }
        } else {
            auto node = AccessibleNode::get(rootNode.get());
            if (rootNode.get()) g_object_unref(rootNode.get());
            ret.push_back((node));
        }
    }
    return ret;
}
*/
/*
void AtspiAccessibleWatcher::clearWindowList()
{
    std::unique_lock<std::mutex> lock(mLock);

    std::for_each(mActivatedWindowSet.begin(), mActivatedWindowSet.end(), [](auto window){
        g_object_unref(window);
    });
    mActivatedWindowSet.clear();

    mWindowSet.clear();
}
*/