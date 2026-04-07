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
#include "AtspiAccessibleWatcher.h"
#else
#include "MockDeviceImpl.h"
#endif
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

class KeyAction {
public:
    KeyAction(IDevice *deviceImpl)
    : mDeviceImpl(deviceImpl) {}
    virtual ~KeyAction(){};

    virtual bool perform(KeyRequestType type) = 0;
protected:
    IDevice *mDeviceImpl;
};

class BackKeyAction : public KeyAction {
public:
    BackKeyAction(IDevice *deviceImpl)
    : KeyAction(deviceImpl) {}

    virtual ~BackKeyAction(){};

    bool perform(KeyRequestType type) override {
        return mDeviceImpl->pressKeyCode("XF86Back", type);
    }
};

class HomeKeyAction : public KeyAction {
public:
    HomeKeyAction(IDevice *deviceImpl)
    : KeyAction(deviceImpl) {}

    virtual ~HomeKeyAction(){};

    bool perform(KeyRequestType type) override {
        return mDeviceImpl->pressKeyCode("XF86Home", type);
    }
};

class MenuKeyAction : public KeyAction {
public:
    MenuKeyAction(IDevice *deviceImpl)
    : KeyAction(deviceImpl) {}

    virtual ~MenuKeyAction(){};

    bool perform(KeyRequestType type) override {
        return mDeviceImpl->pressKeyCode("XF86Menu", type);
    }
};

class VolUpKeyAction : public KeyAction {
public:
    VolUpKeyAction(IDevice *deviceImpl)
    : KeyAction(deviceImpl) {}

    virtual ~VolUpKeyAction(){};

    bool perform(KeyRequestType type) override {
        return mDeviceImpl->pressKeyCode("XF86AudioRaiseVolume", type);
    }
};

class VolDownKeyAction : public KeyAction {
public:
    VolDownKeyAction(IDevice *deviceImpl)
    : KeyAction(deviceImpl) {}

    virtual ~VolDownKeyAction(){};

    bool perform(KeyRequestType type) override {
        return mDeviceImpl->pressKeyCode("XF86AudioLowerVolume", type);
    }
};

class PowerKeyAction : public KeyAction {
public:
    PowerKeyAction(IDevice *deviceImpl)
    : KeyAction(deviceImpl) {}

    virtual ~PowerKeyAction(){};

    bool perform(KeyRequestType type) override {
        return mDeviceImpl->pressKeyCode("XF86PowerOff", type);
    }
};

std::once_flag UiDevice::mOnceFlag;

UiDevice::UiDevice() : UiDevice(nullptr) {}

UiDevice::UiDevice(IDevice *impl)
    : mDeviceImpl(impl), mWaiter(new Waiter{this})
{
    LOGI("UiDevice constructor");
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
            device.reset(new UiDevice(new Mock::MockDeviceImpl()));
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

std::vector<std::shared_ptr<UiObject>> UiDevice::getMatches(
    const std::shared_ptr<UiSelector> selector, const bool earlyReturn) const
{
    std::vector<std::shared_ptr<UiObject>> ret{};

    auto rootNodes = getWindowRoot();
    for (const auto &window : rootNodes) {
        auto nodes = window->getMatches(selector, earlyReturn);
        for (auto &node : nodes)
            ret.push_back(std::make_shared<UiObject>(getInstance(), nullptr, node));
    }
    return ret;
}

std::vector<std::shared_ptr<UiObject>> UiDevice::getMatchesInMatches(
    const std::shared_ptr<UiSelector> firstSelector, const std::shared_ptr<UiSelector> secondSelector, const bool earlyReturn) const
{
    std::vector<std::shared_ptr<UiObject>> ret{};

    auto rootNodes = getWindowRoot();
    for (const auto &window : rootNodes) {
        auto nodes = window->getMatchesInMatches(firstSelector, secondSelector, earlyReturn);
        for (auto &node : nodes)
            ret.push_back(std::make_shared<UiObject>(getInstance(), nullptr, node));
    }
    return ret;
}

std::unordered_map<std::string, std::shared_ptr<UiObject>> UiDevice::getSnapshot() const
{
    std::unordered_map<std::string, std::shared_ptr<UiObject>> snapshot{};
    std::unordered_set<uintptr_t> dedupNodes{};
    int snapshotIndex = 0;

    auto rootNodes = getWindowRoot();
    for (const auto &rootNode : rootNodes) {
        if (!rootNode) continue;

        auto rootObj = std::make_shared<UiObject>(getInstance(), nullptr, rootNode);
        auto rootSnapshot = rootObj->getSnapshot();
        for (const auto &entry : rootSnapshot) {
            auto snapshotObj = entry.second;
            if (!snapshotObj) continue;

            auto rawNode = reinterpret_cast<uintptr_t>(snapshotObj->getAccessibleNode().get());
            if (!dedupNodes.insert(rawNode).second) continue;

            snapshot["e" + std::to_string(++snapshotIndex)] = snapshotObj;
        }
    }

    return snapshot;
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
    const A11yEvent type, const int timeout, const std::string packageName) const
{
    return executeAndWaitForEvents(NULL, type, timeout, packageName, 0);
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
    const std::string keycode, const A11yEvent type, const int timeout, const std::string packageName) const
{
    std::unique_ptr<SendKeyRunnable> cmd = std::make_unique<SendKeyRunnable>(keycode);
    return executeAndWaitForEvents(cmd.get(), type, timeout, packageName, 0);
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

bool UiDevice::mouseDown(const int x, const int y, const int button)
{
    bool result =  mDeviceImpl->mouseDown(x, y, button);
    waitForIdle();
    return result;
}

bool UiDevice::mouseMove(const int x, const int y, const int button)
{
    bool result =  mDeviceImpl->mouseMove(x, y, button);
    waitForIdle();
    return result;
}

bool UiDevice::mouseUp(const int x, const int y, const int button)
{
    bool result =  mDeviceImpl->mouseUp(x, y, button);
    waitForIdle();
    return result;
}

bool UiDevice::generateKey(KeyType keyType, KeyRequestType keyReqestType)
{
    std::unique_ptr<KeyAction> keyAction;

    if (keyType == KeyType::BACK)
        keyAction = std::make_unique<BackKeyAction>(mDeviceImpl);
    else if (keyType == KeyType::HOME)
        keyAction = std::make_unique<HomeKeyAction>(mDeviceImpl);
    else if (keyType == KeyType::MENU)
        keyAction = std::make_unique<MenuKeyAction>(mDeviceImpl);
    else if (keyType == KeyType::VOLUP)
        keyAction = std::make_unique<VolUpKeyAction>(mDeviceImpl);
    else if (keyType == KeyType::VOLDOWN)
        keyAction = std::make_unique<VolDownKeyAction>(mDeviceImpl);
    else if (keyType == KeyType::POWER)
        keyAction = std::make_unique<PowerKeyAction>(mDeviceImpl);

    bool result = keyAction->perform(keyReqestType);

    return result;
}

bool UiDevice::pressBack(KeyRequestType type)
{
    return generateKey(KeyType::BACK, type);
}

bool UiDevice::pressHome(KeyRequestType type)
{
    return generateKey(KeyType::HOME, type);
}

bool UiDevice::pressMenu(KeyRequestType type)
{
    return generateKey(KeyType::MENU, type);
}

bool UiDevice::pressVolUp(KeyRequestType type)
{
    return generateKey(KeyType::VOLUP, type);
}

bool UiDevice::pressVolDown(KeyRequestType type)
{
    return generateKey(KeyType::VOLDOWN, type);
}

bool UiDevice::pressPower(KeyRequestType type)
{
    return generateKey(KeyType::POWER, type);
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

bool UiDevice::getExternalAppLaunched()
{
    auto ret = this->getWindowRoot();
    return (ret.size() > 0) ? false : true;
}

bool UiDevice::registerCallback(const A11yEvent type, EventHandler cb, void *data) const
{
    return AccessibleWatcher::getInstance()->registerCallback(type, std::move(cb), data);
}

bool UiDevice::clearCallback() const
{
    return AccessibleWatcher::getInstance()->clearCallback();
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
