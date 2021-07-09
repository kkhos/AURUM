#include "Aurum.h"

#include "AtspiAccessibleWatcher.h"
#include "AtspiAccessibleApplication.h"
#include "AtspiAccessibleWindow.h"
#include "AtspiAccessibleNode.h"
#include "AtspiWrapper.h"

#include <algorithm>
#include <chrono>
#include <thread>
#include <iostream>

std::vector<std::shared_ptr<A11yEventInfo>> AtspiAccessibleWatcher::mEventQueue;
GThread *AtspiAccessibleWatcher::mEventThread = nullptr;
std::mutex AtspiAccessibleWatcher::mMutex = std::mutex{};

static bool iShowingNode(AtspiAccessible *node)
{
    char *name = NULL;
    if (node) name = AtspiWrapper::Atspi_accessible_get_name(node, NULL);
    else return false;

    LOGI("isShowing %s", name);
    auto stateSet = AtspiWrapper::Atspi_accessible_get_state_set(node);

    if (AtspiWrapper::Atspi_state_set_contains(stateSet, ATSPI_STATE_ACTIVE)
        && AtspiWrapper::Atspi_state_set_contains(stateSet, ATSPI_STATE_SHOWING)) {
        LOGI("active and showing %p %s", node, name);
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
    LOGI("findActiveNode %p %d/%d", node, depth, max_depth);

    std::vector<AtspiAccessible *> ret{};

    if (iShowingNode(node)) {
        char *name = AtspiWrapper::Atspi_accessible_get_name(node, NULL);
        if (name) {
            LOGI("%s", name);
            free(name);
        }
        ret.push_back(node);
        return ret;
    }

    if (depth >= max_depth) return ret;

    int nchild = AtspiWrapper::Atspi_accessible_get_child_count(node, NULL);
    if (nchild <= 0) return ret;

    LOGI("findActiveNode node %p has %d children", node, nchild);
    for (int i = 0; i < nchild; i++) {
        AtspiAccessible* child = AtspiWrapper::Atspi_accessible_get_child_at_index(node, i, NULL);
        LOGI("a child found @ %d : %p", i, child);
        std::vector<AtspiAccessible *> childRet = findActiveNode(child, depth + 1, max_depth);
        ret.insert(ret.end(), childRet.begin(), childRet.end());
        g_object_unref(child);
    }

    return ret;
}

static gpointer _event_thread_loop (gpointer data)
{
    LOGI("event thread start");
    AtspiEventListener * listener =
        atspi_event_listener_new(AtspiAccessibleWatcher::onAtspiEvents, data, NULL);

    atspi_event_listener_register(listener, "window:", NULL);
    atspi_event_listener_register(listener, "object:", NULL);

    atspi_event_main();

    LOGI("event thread end");
    atspi_event_listener_deregister(listener, "object:", NULL);
    atspi_event_listener_deregister(listener, "window:", NULL);

    g_object_unref(listener);

    return NULL;
}
AtspiAccessibleWatcher::AtspiAccessibleWatcher()
: mDbusProxy{nullptr}
{
    GVariant *enabled_variant = nullptr, *result = nullptr;
    GError *  error = nullptr;
    atspi_set_main_context (g_main_context_default ());
    atspi_init();

    mEventThread = g_thread_new("AtspiEventThread", _event_thread_loop, this);

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
    g_error_free(error);
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

    g_object_unref(mDbusProxy);
    g_variant_unref(enabled_variant);
    g_variant_unref(result);
    g_error_free(error);

    atspi_event_quit();
    g_thread_join(mEventThread);
    atspi_exit();
}

void AtspiAccessibleWatcher::onAtspiEvents(AtspiEvent *event, void *user_data)
{
    if (!event->source)
    {
        return;
    }
    char *name = NULL, *pkg = NULL;
    AtspiAccessibleWatcher *instance = (AtspiAccessibleWatcher *)user_data;
    name = AtspiWrapper::Atspi_accessible_get_name(event->source, NULL);

    AtspiAccessible *app = AtspiWrapper::Atspi_accessible_get_application(event->source, NULL);
    if (app)
    {
        pkg = AtspiWrapper::Atspi_accessible_get_name(app, NULL);
        g_object_unref(app);
    }
    else
        pkg = strdup("");

    mEventQueue.push_back(std::make_shared<A11yEventInfo>(std::string(event->type), std::string(name), std::string(pkg)));

    if (!strcmp(event->type, "object:state-changed:defunct")) {
         instance->onObjectDefunct(
            static_cast<AtspiAccessible *>(event->source));
    }
    if (name) free(name);
    if (pkg) free(pkg);
}

void AtspiAccessibleWatcher::print_debug()
{
    LOGI("activatewindowlist-------------------");
    std::for_each(mActivatedWindowList.begin(), mActivatedWindowList.end(), [](auto acc){
        LOGI("child:%p", acc);
    });

    LOGI("mActivatedApplicationList--------------------------");
    std::for_each(mActivatedApplicationList.begin(), mActivatedApplicationList.end(), [](auto acc){
        LOGI("child:%p", acc);
    });

    LOGI("mWindowSet------------------------------");
    std::for_each(mWindowSet.begin(), mWindowSet.end(), [](auto acc){
        LOGI("child:%p", acc);
    });
    LOGI("------------------------------");
}

void AtspiAccessibleWatcher::onWindowActivated(AtspiAccessible *node,
                            WindowActivateInfoType type)
{
    LOGI("onWindowActivated obj:%p", node);
    notifyAll((int)EventType::Window, (int)WindowEventType::WindowActivated, node);
}

void AtspiAccessibleWatcher::onWindowDeactivated(AtspiAccessible *node)
{
    LOGI("onWindowDeactivated obj:%p", node);
    notifyAll((int)EventType::Window, (int)WindowEventType::WindowDeactivated, node);
}

void AtspiAccessibleWatcher::onWindowCreated(AtspiAccessible *node)
{
    LOGI("onWindowCreated obj:%p", node);
    notifyAll((int)EventType::Window, (int)WindowEventType::WindowCreated, node);
}

void AtspiAccessibleWatcher::onWindowDestroyed(AtspiAccessible *node)
{
    LOGI("onWindowDestroyed obj:%p", node);
    notifyAll((int)EventType::Window, (int)WindowEventType::WindowDestroyed, node);
}

void AtspiAccessibleWatcher::onVisibilityChanged(AtspiAccessible *node, bool visible)
{
    LOGI("onVisibilityChanged obj:%p", node);
    notifyAll((int)EventType::Object, (int)ObjectEventType::ObjectStateVisible, node);
}

void AtspiAccessibleWatcher::onObjectDefunct(AtspiAccessible *node)
{
    LOGI("onObjectDefunct obj:%p", node);
    notifyAll((int)EventType::Object, (int)ObjectEventType::ObjectStateDefunct, node);
}

int AtspiAccessibleWatcher::getApplicationCount(void) const
{
    AtspiAccessible *root = AtspiWrapper::Atspi_get_desktop(0);
    int nchild = AtspiWrapper::Atspi_accessible_get_child_count(root, NULL);
    g_object_unref(root);

    if (nchild <= 0) return 0;
    return nchild;
}

std::shared_ptr<AccessibleApplication> AtspiAccessibleWatcher::getApplicationAt(int index) const
{
    AtspiAccessible *root = AtspiWrapper::Atspi_get_desktop(0);
    AtspiAccessible* child = AtspiWrapper::Atspi_accessible_get_child_at_index(root, index, NULL);
    g_object_unref(root);
    return std::make_shared<AtspiAccessibleApplication>(std::make_shared<AtspiAccessibleNode>(child));
}

std::vector<std::shared_ptr<AccessibleApplication>> AtspiAccessibleWatcher::getApplications(void) const
{
    std::vector<std::shared_ptr<AccessibleApplication>> ret{};
    AtspiAccessible *root = AtspiWrapper::Atspi_get_desktop(0);
    int nchild = AtspiWrapper::Atspi_accessible_get_child_count(root, NULL);
    if (nchild <= 0) {
        g_object_unref(root);
        return ret;
    }

    for (int i = 0; i < nchild; i++){
        AtspiAccessible* child = AtspiWrapper::Atspi_accessible_get_child_at_index(root, i, NULL);
        if (child) {
            ret.push_back(std::make_shared<AtspiAccessibleApplication>(std::make_shared<AtspiAccessibleNode>(child)));
        }
    }
    g_object_unref(root);
    return ret;
}

#define COMPARE(A, B) \
    (B != A11yEvent::EVENT_NONE) && ((A & B) == B)

bool AtspiAccessibleWatcher::executeAndWaitForEvents(const Runnable *cmd, const A11yEvent type, const int timeout)
{
    mMutex.lock();
    mEventQueue.clear();
    mMutex.unlock();
    if (cmd)
        cmd->run();

    std::chrono::system_clock::time_point start =
        std::chrono::system_clock::now();
	while (true)
    {
        std::vector<std::shared_ptr<A11yEventInfo>> localEvents;
        mMutex.lock();
        localEvents.assign(mEventQueue.begin(), mEventQueue.end());
        mEventQueue.clear();
        mMutex.unlock();

        if (!localEvents.empty())
        {
            for (const auto &event : localEvents) {
                if (COMPARE(type, event->getEvent()))
                {
                    LOGI("type %d == %d name %s pkg %s",static_cast<int>(type), static_cast<int>(event->getEvent()), event->getName().c_str(), event->getPkg().c_str()); 
                    return true;
                }
            }
        }
        if ((std::chrono::system_clock::now() - start) >
            std::chrono::milliseconds{timeout})
            break;
        std::this_thread::sleep_for(
            std::chrono::milliseconds{100});
    }

    return false;
}

bool AtspiAccessibleWatcher::removeFromActivatedList(AtspiAccessible *node)
{
    LOGI("remove from activelist node %p", node);
    mActivatedWindowList.remove_if([&](auto &n) { return n == node; });

    AtspiAccessible *app = AtspiWrapper::Atspi_accessible_get_application(node, NULL);
    LOGI("node:%p, app:%p", node, app);
    if (app) {
        mActivatedApplicationList.remove_if([&](auto &n) { return n == app; });
        g_object_unref(app);
    }
    return true;
}

bool AtspiAccessibleWatcher::addToActivatedList(AtspiAccessible *node)
{
    LOGI("add to activelist node %p", node);
    mActivatedWindowList.remove_if([&](auto &n) { return n == node; });
    mActivatedWindowList.push_front(node);

    auto iter = mWindowSet.find(node);
    if ( iter == mWindowSet.end()) mWindowSet.insert(node);

    AtspiAccessible *app = AtspiWrapper::Atspi_accessible_get_application(node, NULL);
    LOGI("node:%p, app:%p", node, app);
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
