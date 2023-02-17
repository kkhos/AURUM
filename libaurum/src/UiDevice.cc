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

#ifdef TIZEN
#include "TizenDeviceImpl.h"
#endif
#include "MockDeviceImpl.h"
#include "AtspiAccessibleWatcher.h"
#include <unistd.h>
#include <utility>
#include <vector>
#include <chrono>
#include <thread>
#include <algorithm>
#include <iostream>
#include <gio/gio.h>

using namespace Aurum;
using namespace AurumInternal;

#define WM_BUS_NAME "org.enlightenment.wm"
#define WM_OBJECT_PATH "/org/enlightenment/wm"
#define WM_INTERFACE_NAME "org.enlightenment.wm.proc"
#define WM_METHOD_NAME_INFO "GetVisibleWinInfo_v2"

std::vector<std::shared_ptr<TizenWindow>> UiDevice::mTizenWindows;
std::once_flag UiDevice::mOnceFlag;
static GDBusConnection *system_conn;

#ifdef MQTT_ENABLED
std::shared_ptr<ScreenAnalyzerWatcher> UiDevice::mSAWatcher;
#endif

UiDevice::UiDevice() : UiDevice(nullptr) {}

UiDevice::UiDevice(IDevice *impl)
    : mDeviceImpl(impl), mWaiter(new Waiter{this})
{
    LOGI("UiDevice constructor");
    mIsWithSA = false;
#ifdef MQTT_ENABLED
    mSAWatcher = std::make_shared<ScreenAnalyzerWatcher>();
#endif
    LOGI("UiDevice constructor finish");
}

UiDevice::~UiDevice()
{
    delete mDeviceImpl;
    delete mWaiter;
}

std::shared_ptr<UiDevice> UiDevice::getInstance(IDevice *deviceImpl)
{
    static std::shared_ptr<UiDevice> device{nullptr};
    std::call_once(mOnceFlag, [deviceImpl] {
        if (deviceImpl) {
            device.reset(new UiDevice(deviceImpl));
        } else {
#ifdef TIZEN
            device.reset(new UiDevice(new TizenDeviceImpl()));
#else
            device.reset(new UiDevice(new MockDeviceImpl()));
#endif
        }
    });

    return device;
}

std::vector<std::shared_ptr<TizenWindow>> UiDevice::getTizenWindowInfo() const
{
    GError *err = NULL;
    GDBusMessage *msg;
    GDBusMessage *reply;
    GDBusConnection *conn;
    GVariant *body;
    GVariantIter *iter = NULL;
    int idx = 0;
    int pid;
    int x;
    int y;
    int w;
    int h;
    gboolean transformed;
    gboolean alpha;
    int opaque;
    int visibility;
    gboolean focused;
    gboolean mapped;
    int layer;
    char *name;

    mTizenWindows.clear();

    if (system_conn == NULL) {
        conn = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &err);
        if (conn == NULL) {
            LOGE("g_bus_get_sync() is failed. %s", err->message);
            g_error_free(err);
            return mTizenWindows;
        }
        system_conn = conn;
    }

    msg = g_dbus_message_new_method_call(WM_BUS_NAME,
            WM_OBJECT_PATH,
            WM_INTERFACE_NAME,
            WM_METHOD_NAME_INFO);
    if (msg == NULL) {
        LOGE("g_dbus_message_new_method_call() is failed.");
        return mTizenWindows;
    }

    reply = g_dbus_connection_send_message_with_reply_sync(system_conn, msg,
            G_DBUS_SEND_MESSAGE_FLAGS_NONE, -1, NULL, NULL, &err);

    if (!reply) {
        if (err != NULL) {
            LOGE("Failed to get info [%s]", err->message);
            g_error_free(err);
        }
        goto out;
    }

    body = g_dbus_message_get_body(reply);
    if (!body) {
        LOGE("Failed to get body");
        goto out;
    }

    g_variant_get(body, "(a(iiiiibbiibbis))", &iter);
    if (!iter) {
        LOGE("Failed to get iter");
        goto out;
    }

    LOGI("%-3s | %-6s | %-4s | %-4s | %-4s | %-4s | %-5s | %-5s | %-6s | %-3s | %-7s | %-6s | %-5s | %-20s", "No" ,"PID", "X", "Y", "W", "H", "Trans", "Alpha", "Opaque", "Vis", "Focused", "Mapped", "Layer", "Name");
    while (g_variant_iter_loop(iter, "(iiiiibbiibbis)",
                &pid,
                &x,
                &y,
                &w,
                &h,
                &transformed,
                &alpha,
                &opaque,
                &visibility,
                &focused,
                &mapped,
                &layer,
                &name)) {
        LOGI("%-3d | %-6d | %-4d | %-4d | %-4d | %-4d | %-5d | %-5d | %-6d | %-3d | %-7d | %-6d | %-5d | %-20s", idx++, pid, x,y,w,h, transformed, alpha, opaque, visibility, focused, mapped, layer, name);
        if (visibility == 0 && pid > 0)
        {
            Rect<int> geometry = {x,  y, w, h};
            std::string winName(name);
            mTizenWindows.push_back(std::make_shared<Aurum::TizenWindow>(pid, geometry, transformed, alpha, opaque, visibility, focused, mapped, layer, winName));
        }
    }

out:
    if (iter)
        g_variant_iter_free(iter);
    if (msg)
        g_object_unref(msg);
    if (reply)
        g_object_unref(reply);

    return mTizenWindows;
}

std::vector<std::shared_ptr<AccessibleNode>> UiDevice::getWindowRoot() const
{
    bool dup;
    LOGI("Request window info");
    getTizenWindowInfo();

    std::vector<std::shared_ptr<AccessibleNode>> ret{};

    auto apps = AccessibleWatcher::getInstance()->getApplications();
    for (auto app : apps)
    {
        app->getAccessibleNode()->updateName();
        app->getAccessibleNode()->updatePid();
        LOGI("App(%s) Pid(%d)", app->getPackageName().c_str(), app->getAccessibleNode()->getPid());
    }

    for (auto tWin : mTizenWindows)
    {
        LOGI("Visible win (%d) (%d %d %d %d) (%s)", tWin->getPid(), tWin->getWindowGeometry().mTopLeft.x, tWin->getWindowGeometry().mTopLeft.y, tWin->getWindowGeometry().width(),
            tWin->getWindowGeometry().height(), tWin->getName().c_str());

        for (auto app : apps)
        {
            dup = false;
            if (app->getAccessibleNode()->getPid() == tWin->getPid())
            {
                for (const auto &retWin : ret)
                {
                    retWin->updatePid();
                    LOGI("Pid Dup check in vector (%d) target (%d)", retWin->getPid(), tWin->getPid());
                    if (retWin->getPid() == tWin->getPid())
                    {
                        dup = true;
                        break;
                    }
                }

                if (!dup)
                {
                    LOGI("Actvie App : (%s) (%d)", tWin->getName().c_str(), tWin->getPid());
                    auto wins = app->getWindows();
                    std::transform(wins.begin(), wins.end(), std::back_inserter(ret),
                        [&](std::shared_ptr<AccessibleWindow> window){
                            LOGI("Target window add pkg: (%s), name (%s)", window->getAccessibleNode()->getPkg().c_str(), window->getTitle().c_str());
                            return window->getAccessibleNode();
                        }
                    );
                }
            }
        }
    }

    return ret;
}

bool UiDevice::hasObject(const std::shared_ptr<UiSelector> selector) const
{
    auto rootNodes = getWindowRoot();
    for (const auto &node : rootNodes) {
        const std::shared_ptr<AccessibleNode> foundNode =
            Comparer::findObject(getInstance(), selector, node);
        if (foundNode) return true;
    }

    return false;
}

std::shared_ptr<UiObject> UiDevice::findObject(const std::shared_ptr<UiSelector> selector) const
{
    auto rootNodes = getWindowRoot();
    for (const auto &node : rootNodes) {
        const std::shared_ptr<AccessibleNode> foundNode =
            Comparer::findObject(getInstance(), selector, node);
        if (foundNode)
            return std::make_shared<UiObject>(getInstance(), selector, foundNode);
    }
    return std::shared_ptr<UiObject>{nullptr};
}


std::vector<std::shared_ptr<UiObject>> UiDevice::findObjects(
    const std::shared_ptr<UiSelector> selector) const
{
    std::vector<std::shared_ptr<UiObject>> ret{};
    auto rootNodes = getWindowRoot();
    for (const auto &window : rootNodes) {
        std::vector<std::shared_ptr<AccessibleNode>> nodes =
            Comparer::findObjects(getInstance(), selector, window);
        for (auto &node : nodes)
            ret.push_back(std::make_shared<UiObject>(getInstance(), selector, node));
    }
    return ret;
}
bool UiDevice::waitFor(
    const std::function<bool(const ISearchable *)> condition) const
{
    return mWaiter->waitFor(condition);
}

std::shared_ptr<UiObject> UiDevice::waitFor(
    const std::function<std::shared_ptr<UiObject>(const ISearchable *)>
        condition) const
{
    return mWaiter->waitFor(condition);
}
bool UiDevice::waitForIdle() const
{
    std::this_thread::sleep_for(std::chrono::milliseconds{167});
    return true;
}

bool UiDevice::waitForEvents(
    const A11yEvent type, const int timeout) const
{
    return executeAndWaitForEvents(NULL, type, timeout, std::string(), NULL, 0);
}

bool UiDevice::executeAndWaitForEvents
    (const Runnable *cmd, const A11yEvent type, const int timeout, const std::string packageName, std::shared_ptr<AccessibleNode> obj, const int count)  const
{
    //FIXME: Need to get top window
    auto wins = this->getWindowRoot();

    return AccessibleWatcher::getInstance()->executeAndWaitForEvents(cmd, type, timeout, packageName, wins[0], count);
}

bool UiDevice::sendKeyAndWaitForEvents(
    const std::string keycode, const A11yEvent type, const int timeout) const
{
    std::unique_ptr<SendKeyRunnable> cmd = std::make_unique<SendKeyRunnable>(keycode);
    return executeAndWaitForEvents(cmd.get(), type, timeout, std::string(), NULL, 0);
}

bool UiDevice::click(const int x, const int y)
{
    bool result =  mDeviceImpl->click(x, y);
    waitForIdle();
    return result;
}

bool UiDevice::click(const int x, const int y, const unsigned int durationMs)
{
    bool result = mDeviceImpl->click(x, y, durationMs);
    waitForIdle();
    return result;
}

bool UiDevice::drag(const int sx, const int sy, const int ex, const int ey,
                    const int steps, const int durationMs)
{
    bool result =  mDeviceImpl->drag(sx, sy, ex, ey, steps, durationMs);
    waitForIdle();
    return result;
}

int UiDevice::touchDown(const int x, const int y)
{
    int seq =  mDeviceImpl->touchDown(x, y);
    return seq;
}

bool UiDevice::touchMove(const int x, const int y, const int seq)
{
    bool result =  mDeviceImpl->touchMove(x, y, seq);
    return result;
}

bool UiDevice::touchUp(const int x, const int y, const int seq)
{
    bool result =  mDeviceImpl->touchUp(x, y, seq);
    waitForIdle();
    return result;
}

bool UiDevice::wheelUp(int amount, const int durationMs)
{
    bool result =  mDeviceImpl->wheelUp(amount, durationMs);
    waitForIdle();
    return result;
}

bool UiDevice::wheelDown(int amount, const int durationMs)
{
    bool result =  mDeviceImpl->wheelDown(amount, durationMs);
    waitForIdle();
    return result;
}

bool UiDevice::pressBack(KeyRequestType type)
{
    bool result =  mDeviceImpl->pressBack(type);
    waitForIdle();
    return result;
}

bool UiDevice::pressHome(KeyRequestType type)
{
    bool result =  mDeviceImpl->pressHome(type);
    waitForIdle();
    return result;
}

bool UiDevice::pressMenu(KeyRequestType type)
{
    bool result =  mDeviceImpl->pressMenu(type);
    waitForIdle();
    return result;
}

bool UiDevice::pressVolUp(KeyRequestType type)
{
    bool result =  mDeviceImpl->pressVolUp(type);
    waitForIdle();
    return result;
}

bool UiDevice::pressVolDown(KeyRequestType type)
{
    bool result =  mDeviceImpl->pressVolDown(type);
    waitForIdle();
    return result;
}

bool UiDevice::pressPower(KeyRequestType type)
{
    bool result =  mDeviceImpl->pressPower(type);
    waitForIdle();
    return result;
}

bool UiDevice::pressKeyCode(std::string keycode, KeyRequestType type)
{
    bool result =  mDeviceImpl->pressKeyCode(keycode, type);
    return result;
}

bool UiDevice::repeatKeyCode(std::string keycode, int intervalMs, int durationMs)
{
    bool result =  mDeviceImpl->repeatKeyCode(keycode, intervalMs, durationMs);
    return result;
}

bool UiDevice::takeScreenshot(std::string path, float scale, int quality)
{
    return mDeviceImpl->takeScreenshot(path, scale, quality);
}

long long UiDevice::getSystemTime(TimeRequestType type)
{
    return mDeviceImpl->getSystemTime(type);
}

const Size2D<int> UiDevice::getScreenSize()
{
    return mDeviceImpl->getScreenSize();
}

#ifdef MQTT_ENABLED
std::vector<std::shared_ptr<SaObject>> UiDevice::getSaObject()
{
    return mSAWatcher->GetSaObjects();
}

std::shared_ptr<ScreenAnalyzerWatcher> UiDevice::getSAWatcher()
{
    return mSAWatcher;
}
#endif

void UiDevice::RequestScreenAnalyze()
{
#ifdef MQTT_ENABLED
    mSAWatcher->PublishData();
#endif
}

bool UiDevice::getExternalAppLaunched()
{
    auto ret = this->getWindowRoot();
    return (ret.size() > 0) ? false : true;
}

void UiDevice::setWithScreenAnalyzer(bool withScreenAnalyzer)
{
    mIsWithSA = withScreenAnalyzer;
}

bool UiDevice::getWithScreenAnalyzer()
{
    return mIsWithSA;
}

bool UiDevice::registerCallback(const A11yEvent type, EventHandler cb, void *data) const
{
    return AccessibleWatcher::getInstance()->registerCallback(type, cb, data);
}