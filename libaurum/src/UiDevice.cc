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
#include <unordered_set>
#include <unordered_map>

using namespace Aurum;
using namespace AurumInternal;

std::once_flag UiDevice::mOnceFlag;

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

std::vector<std::shared_ptr<AccessibleNode>> UiDevice::getWindowRoot() const
{
    return mDeviceImpl->getWindowRoot();
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
        std::vector<std::shared_ptr<AccessibleNode>> nodes{};
        Comparer::findObjects(nodes, getInstance(), selector, window);

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
    return executeAndWaitForEvents(NULL, type, timeout, std::string(), 0);
}

//FIXME: obj only need for idle event
bool UiDevice::executeAndWaitForEvents
    (const Runnable *cmd, const A11yEvent type, const int timeout, const std::string packageName, const int count)  const
{
    std::vector<std::shared_ptr<AccessibleNode>> wins;

    //FIXME: Need to get top window
    if (type != A11yEvent::EVENT_NONE &&  (A11yEvent::EVENT_WINDOW_RENDER_POST & type) == type)
    {
        wins = this->getWindowRoot();
        return AccessibleWatcher::getInstance()->executeAndWaitForEvents(cmd, type, timeout, packageName, wins[0], count);
    }

    return AccessibleWatcher::getInstance()->executeAndWaitForEvents(cmd, type, timeout, packageName, NULL, count);
}

bool UiDevice::sendKeyAndWaitForEvents(
    const std::string keycode, const A11yEvent type, const int timeout) const
{
    std::unique_ptr<SendKeyRunnable> cmd = std::make_unique<SendKeyRunnable>(keycode);
    return executeAndWaitForEvents(cmd.get(), type, timeout, std::string(), 0);
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

bool UiDevice::takeScreenshot(std::string path, bool asPixels, void **pixels, int angle)
{
    angle = getWindowAngle();
    return mDeviceImpl->takeScreenshot(path, asPixels, pixels, angle);
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

int UiDevice::getWindowAngle()
{
    auto wins = getWindowRoot();

    if (wins.size() > 0) return wins[0]->getWindowAngle();

    return 0;
}

int UiDevice::getTargetAngle()
{
    auto wins = getWindowRoot();

    if (wins.size() > 0) return wins[0]->getTargetAngle();

    return 0;
}
