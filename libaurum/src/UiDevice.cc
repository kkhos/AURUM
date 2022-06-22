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

#include "ScreenAnalyzerWatcher.h"

using namespace Aurum;
using namespace AurumInternal;

std::shared_ptr<ScreenAnalyzerWatcher> scw;

UiDevice::UiDevice() : UiDevice(nullptr) {}

UiDevice::UiDevice(IDevice *impl)
    : mDeviceImpl(impl), mWaiter(new Waiter{this})
{
    scw = std::make_shared<ScreenAnalyzerWatcher>();
}

UiDevice::~UiDevice()
{
    delete mDeviceImpl;
    delete mWaiter;
}

std::shared_ptr<UiDevice> UiDevice::getInstance(IDevice *deviceImpl)
{
    static std::shared_ptr<UiDevice> device{nullptr};

    if (deviceImpl) {
        device.reset(new UiDevice(deviceImpl));
    } else {
        if (device) return device;
        else {
#ifdef TIZEN
            device.reset(new UiDevice(new TizenDeviceImpl()));
#else
            device.reset(new UiDevice(new MockDeviceImpl()));
#endif
        }
    }

    return device;
}

std::vector<std::shared_ptr<AccessibleNode>> UiDevice::getWindowRoot() const
{
    std::vector<std::shared_ptr<AccessibleNode>> ret{};

    auto appsMap = AccessibleWatcher::getInstance()->getActiveAppMap();
    LOGI("activeAppMap.size: %d" , (int)appsMap.size());
    for (auto itr = appsMap.begin(); itr != appsMap.end(); itr++)
    {
        auto activeWindows = itr->second->getActiveWindows();
        std::transform(activeWindows.begin(), activeWindows.end(), std::back_inserter(ret),
            [&](std::shared_ptr<AccessibleWindow> window){
                LOGI("active pkg: %s, window: %s", window->getAccessibleNode()->getPkg().c_str(), window->getTitle().c_str());
                return window->getAccessibleNode();
            }
        );
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
    return executeAndWaitForEvents(NULL, type, timeout);
}

bool UiDevice::executeAndWaitForEvents(
	const Runnable *cmd, const A11yEvent type, const int timeout) const
{
    return AccessibleWatcher::getInstance()->executeAndWaitForEvents(cmd, type, timeout);
}

bool UiDevice::sendKeyAndWaitForEvents(
	const std::string keycode, const A11yEvent type, const int timeout) const
{
    std::unique_ptr<SendKeyRunnable> cmd = std::make_unique<SendKeyRunnable>(keycode);
    return executeAndWaitForEvents(cmd.get(), type, timeout);
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
    waitForIdle();
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

std::vector<std::shared_ptr<SaObject>> UiDevice::getSaObject()
{
    return scw->GetSaObjects();
}

std::shared_ptr<ScreenAnalyzerWatcher> UiDevice::getScw()
{
    return scw;
}

void UiDevice::RequestScreenAnalyze()
{
    scw->PublishData();
}

bool UiDevice::getExternalAppLaunched()
{
    auto ret = this->getWindowRoot();
    return (ret.size() > 0) ? false : true;
}
