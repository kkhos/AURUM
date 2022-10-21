/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *               http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

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
#include <system_info.h>

using namespace Aurum;
using namespace AurumInternal;

#define COMPARE(A, B) \
    (B != A11yEvent::EVENT_NONE) && ((A & B) == B)

std::vector<std::shared_ptr<A11yEventInfo>> AtspiAccessibleWatcher::mEventQueue;
GThread *AtspiAccessibleWatcher::mEventThread = nullptr;
std::mutex AtspiAccessibleWatcher::mMutex = std::mutex{};
GMainLoop *AtspiAccessibleWatcher::mLoop = nullptr;

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
        AtspiAccessible *child = AtspiWrapper::Atspi_accessible_get_child_at_index(node, i, NULL);
        LOGI("a child found @ %d : %p", i, child);
        std::vector<AtspiAccessible *> childRet = findActiveNode(child, depth + 1, max_depth);
        ret.insert(ret.end(), childRet.begin(), childRet.end());
        g_object_unref(child);
    }

    return ret;
}

gpointer AtspiAccessibleWatcher::eventThreadLoop(gpointer data)
{
    AtspiAccessibleWatcher *instance = (AtspiAccessibleWatcher *)data;
    GMainContext *mContext = nullptr;

    LOGI("event thread start");
    AtspiEventListener *listener =
        atspi_event_listener_new(AtspiAccessibleWatcher::onAtspiEvents, instance, NULL);

    atspi_event_listener_register(listener, "window:", NULL);
    atspi_event_listener_register(listener, "object:", NULL);

    mContext = g_main_context_new();
    g_main_context_push_thread_default(mContext);
    atspi_set_main_context (mContext);
    instance->mLoop = g_main_loop_new(mContext, FALSE);

    g_main_loop_run(instance->mLoop);
    LOGI("event thread end");
    atspi_event_listener_deregister(listener, "object:", NULL);
    atspi_event_listener_deregister(listener, "window:", NULL);

    g_object_unref(listener);

    return NULL;
}

AtspiAccessibleWatcher::AtspiAccessibleWatcher()
: mDbusProxy{nullptr}, isTv{false}
{
    GVariant *result = nullptr;
    GError *error = nullptr;

    atspi_init();

    mEventThread = g_thread_new("AtspiEventThread", eventThreadLoop, this);

    mDbusProxy = g_dbus_proxy_new_for_bus_sync(
        G_BUS_TYPE_SESSION, G_DBUS_PROXY_FLAGS_NONE,
        NULL, /* GDBusInterfaceInfo */
        "org.a11y.Bus", "/org/a11y/bus", "org.freedesktop.DBus.Properties",
        NULL, &error);

    result = g_dbus_proxy_call_sync(
        mDbusProxy, "Set",
        g_variant_new("(ssv)", "org.a11y.Status", "IsEnabled", g_variant_new_boolean(true)),
        G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);

    g_variant_unref(result);
    if (error) g_error_free(error);

    int vconfRet;
    char *value;

    vconfRet = system_info_get_platform_string("http://tizen.org/feature/profile", &value);
    if (vconfRet != SYSTEM_INFO_ERROR_NONE) LOGE("Fail to get system profile infomation");
    else {
        if (!strncmp("tv", value, 2)) {
            LOGI("Aurum is working on TV profile");
            isTv = true;
        }

        free(value);
    }
}

AtspiAccessibleWatcher::~AtspiAccessibleWatcher()
{
    GVariant *result = nullptr;
    GError *error = nullptr;

    result = g_dbus_proxy_call_sync(
        mDbusProxy, "Set",
        g_variant_new("(ssv)", "org.a11y.Status", "IsEnabled", g_variant_new_boolean(false)),
        G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);

    g_object_unref(mDbusProxy);
    g_variant_unref(result);
    if (error) g_error_free(error);

    g_main_loop_quit(mLoop);
    g_thread_join(mEventThread);
    atspi_exit();
}

void AtspiAccessibleWatcher::appendApp(AtspiAccessibleWatcher *instance, AtspiAccessible *app, char *pkg)
{
    AtspiWrapper::Atspi_accessible_set_cache_mask(app, ATSPI_CACHE_ALL);
    LOGI("window activated in app(%s)", pkg);
    if (!instance->mActiveAppMap.count(app)) {
        LOGI("add activated window's app in map");
        instance->mActiveAppMap.insert(std::pair<AtspiAccessible *, std::shared_ptr<AccessibleApplication>>(app,
                std::make_shared<AtspiAccessibleApplication>(std::make_shared<AtspiAccessibleNode>(app))));
    }
    else {
        LOGI("app(%s) is already in map", pkg);
    }

    std::string package(pkg);
    if (!package.empty()) {
        if (instance->mXMLDocMap.count(package)) {
            instance->mXMLDocMap.erase(package);
        }
        instance->mXMLDocMap.insert(std::pair<std::string, std::shared_ptr<AurumXML>>(package,
                std::make_shared<AurumXML>(std::make_shared<AtspiAccessibleNode>(app), XMLMutex)));
    }
}

void AtspiAccessibleWatcher::removeApp(AtspiAccessibleWatcher *instance, AtspiAccessible *app, char *pkg)
{
    LOGI("window deactivate in app(%s)", pkg);
    if (instance->mActiveAppMap.count(app)) {
        LOGI("window deactivated delete app(%s) in map", pkg);
        instance->mActiveAppMap.erase(app);
    }
    else {
        LOGE("deactivated window's app(%s) is not in map", pkg);
    }

    if (instance->mXMLDocMap.count(std::string(pkg))) {
        instance->mXMLDocMap.erase(std::string(pkg));
    }

    g_object_unref(app);
}

void AtspiAccessibleWatcher::onAtspiEvents(AtspiEvent *event, void *watcher)
{
    if (!event->source)
    {
        return;
    }
    char *name = NULL, *pkg = NULL;
    AtspiAccessibleWatcher *instance = (AtspiAccessibleWatcher *)watcher;
    name = AtspiWrapper::Atspi_accessible_get_name(event->source, NULL);

    AtspiAccessible *app = AtspiWrapper::Atspi_accessible_get_application(event->source, NULL);
    if (name && app)
    {
        pkg = AtspiWrapper::Atspi_accessible_get_name(app, NULL);
        if (!strncmp(event->type, "window:activate", 15)) instance->appendApp(instance, app, pkg);
        else if (!strncmp(event->type, "window:deactivate", 16)) instance->removeApp(instance, app, pkg);

        // To support focus skipped window
        if (instance->isTv) {
            if (!strncmp(event->type, "window:restore", 14) && (!strncmp(name, "volume-app", 10) || !strncmp(name, "tv-viewer", 9)))
                instance->appendApp(instance, app, pkg);
            else if (!strncmp(event->type, "window:minimize", 15) && (!strncmp(name, "volume-app", 10) || !strncmp(name, "tv-viewer", 9)))
                instance->removeApp(instance, app, pkg);
        }
    }
    else
        pkg = strdup("");

    mMutex.lock();
    auto a11yEvent = std::make_shared<A11yEventInfo>(std::string(event->type), std::string(name), std::string(pkg));
    mEventQueue.push_back(a11yEvent);
    mMutex.unlock();

    if (instance->mHandlers.count(a11yEvent->getEvent())) {
        auto list = instance->mHandlers[a11yEvent->getEvent()];

        bool deleted = false;

        auto it = list.begin();
        while (it != list.end())
        {
            LOGI("Callback call type %s pkg %s", event->type, pkg);
            auto handler = *it;
            bool res = handler->operator()(std::string(pkg));
            if (!res) {
                it = list.erase(it);
                deleted = true;
            } else ++it;
        }
        if (deleted) instance->mHandlers[a11yEvent->getEvent()] = list;
    }

    if (!strcmp(event->type, "object:state-changed:defunct")) {
         instance->onObjectDefunct(
            static_cast<AtspiAccessible *>(event->source));
    }
    if (name) free(name);
    if (pkg) free(pkg);
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
    AtspiAccessible *child = AtspiWrapper::Atspi_accessible_get_child_at_index(root, index, NULL);
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
        AtspiAccessible *child = AtspiWrapper::Atspi_accessible_get_child_at_index(root, i, NULL);
        if (child) {
            ret.push_back(std::make_shared<AtspiAccessibleApplication>(std::make_shared<AtspiAccessibleNode>(child)));
        }
    }
    g_object_unref(root);
    return ret;
}

bool AtspiAccessibleWatcher::executeAndWaitForEvents(const Runnable *cmd, const A11yEvent type, const int timeout, const std::string packageName)
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
                if (COMPARE(type, event->getEvent()) && (packageName.empty() || packageName == event->getPkg()))
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

std::map<AtspiAccessible *, std::shared_ptr<AccessibleApplication>> AtspiAccessibleWatcher::getActiveAppMap(void)
{
    return mActiveAppMap;
}

std::map<std::string, std::shared_ptr<AurumXML>> AtspiAccessibleWatcher::getXMLDocMap(void)
{
    bool isFirstWaiting = true;
    while(!XMLMutex.try_lock())
    {
        if(isFirstWaiting)
        {
            LOGI("Waiting XMLTree Construct");
            isFirstWaiting = false;
        }
    }
    XMLMutex.unlock();
    return mXMLDocMap;
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

bool AtspiAccessibleWatcher::registerCallback(const A11yEvent type, EventHandler cb, void *data)
{
    auto handler = std::make_shared<A11yEventHandler>(type, cb, data);
    if (mHandlers.count(type)) {
        auto list = mHandlers[type];
        list.push_back(handler);
        mHandlers[type] = list;
    } else {
        std::list<std::shared_ptr<A11yEventHandler>> list;
        list.push_back(handler);
        mHandlers.insert(std::pair<const A11yEvent, std::list<std::shared_ptr<A11yEventHandler>>>(type, list));
    }
    return true;
}
