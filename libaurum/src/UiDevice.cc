#include "UiDevice.h"
#include "AccessibleWatcher.h"
#include "Comparer.h"
#include "DeviceImpl/TizenImpl.h"

#include <unistd.h>
#include <utility>
#include <vector>
#include <chrono>
#include <thread>

UiDevice::UiDevice() : UiDevice(DeviceType::DEFAULT, nullptr) {}

UiDevice::UiDevice(DeviceType type, IDevice *impl)
    : mType(type), mDeviceImpl(impl), mWaiter(new Waiter{this})
{
}

UiDevice::~UiDevice()
{
    delete mDeviceImpl;
    delete mWaiter;
}

UiDevice *UiDevice::getInstance(DeviceType type)
{
    static UiDevice *device = nullptr;
#ifdef TIZEN
    if (!device) device = new UiDevice(type, new TizenImpl());
#endif
    return device;
}

std::vector<std::unique_ptr<AccessibleNode>> UiDevice::getWindowRoot() const
{
    return AccessibleWatcher::getInstance()->getTopNode();
}

bool UiDevice::hasObject(const std::shared_ptr<UiSelector> selector) const
{
    auto root = getWindowRoot();
    for (auto it = root.begin(); it != root.end(); ++it) {
        std::unique_ptr<AccessibleNode> node =
            Comparer::findObject(this, selector, (*it).get());
        if (node != nullptr) return true;
    }

    return false;
}

std::unique_ptr<UiObject> UiDevice::findObject(const std::shared_ptr<UiSelector> selector) const
{
    auto root = getWindowRoot();
    for (auto it = root.begin(); it != root.end(); ++it) {
        std::unique_ptr<AccessibleNode> node =
            Comparer::findObject(this, selector, (*it).get());
        if (node)
            return std::make_unique<UiObject>(this, selector, std::move(node));
    }
    return std::unique_ptr<UiObject>{nullptr};
}

std::vector<std::unique_ptr<UiObject>> UiDevice::findObjects(
    const std::shared_ptr<UiSelector> selector) const
{
    std::vector<std::unique_ptr<UiObject>> ret{};
    auto root = getWindowRoot();
    for (auto it = root.begin(); it != root.end(); ++it) {
        std::vector<std::unique_ptr<AccessibleNode>>          nodes =
            Comparer::findObjects(this, selector, (*it).get());

        for (auto &node : nodes)
            ret.push_back(std::make_unique<UiObject>(this, selector, std::move(node)));
    }
    return ret;
}
bool UiDevice::waitFor(
    const std::function<bool(const ISearchable *)> condition) const
{
    return mWaiter->waitFor(condition);
}

std::unique_ptr<UiObject> UiDevice::waitFor(
    const std::function<std::unique_ptr<UiObject>(const ISearchable *)>
        condition) const
{
    return mWaiter->waitFor(condition);
}
bool UiDevice::waitForIdle() const
{
    std::this_thread::sleep_for(std::chrono::milliseconds{167});
    return true;
}

bool UiDevice::click(const int x, const int y)
{
    bool result =  mDeviceImpl->click(x, y);
    waitForIdle();
    return result;
}

bool UiDevice::click(const int x, const int y, const unsigned int intv)
{
    bool result = mDeviceImpl->click(x, y, intv);
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
