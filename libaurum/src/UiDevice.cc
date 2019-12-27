#include "UiDevice.h"
#include "Accessible.h"
#include "Comparer.h"
#include "DeviceImpl/TM1Impl.h"

#include <unistd.h>
#include <utility>
#include <vector>

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
    if (!device) device = new UiDevice(type, new TM1Impl());
#endif
    return device;
}

const AccessibleNode *UiDevice::getWindowRoot() const
{
    AccessibleNode *root = Accessible::getInstance()->getTopNode();
    // root->print(0,6);
    return root;
}

bool UiDevice::hasObject(const UiSelector *selector) const
{
    AccessibleNode *node =
        Comparer::findObject(this, selector, getWindowRoot());
    if (node != nullptr) return true;
    return false;
}

std::unique_ptr<UiObject> UiDevice::findObject(const UiSelector *selector) const
{
    AccessibleNode *node =
        Comparer::findObject(this, selector, getWindowRoot());
    if (node)
        return std::make_unique<UiObject>(this, selector, node);
    else
        return std::unique_ptr<UiObject>{nullptr};
}

std::vector<std::unique_ptr<UiObject>> UiDevice::findObjects(
    const UiSelector *selector) const
{
    std::vector<std::unique_ptr<UiObject>> ret{};
    std::vector<AccessibleNode *>          nodes =
        Comparer::findObjects(this, selector, getWindowRoot());
    for (const AccessibleNode *node : nodes)
        ret.push_back(std::make_unique<UiObject>(this, selector, node));

    return std::move(ret);
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

bool UiDevice::click(const int x, const int y)
{
    return mDeviceImpl->click(x, y);
}

bool UiDevice::click(const int x, const int y, const unsigned int intv)
{
    return mDeviceImpl->click(x, y, intv);
}

bool UiDevice::drag(const int sx, const int sy, const int ex, const int ey,
                    const int steps)
{
    return mDeviceImpl->drag(sx, sy, ex, ey, steps);
}

bool UiDevice::touchDown(const int x, const int y)
{
    return mDeviceImpl->touchDown(x, y);
}

bool UiDevice::touchMove(const int x, const int y)
{
    return mDeviceImpl->touchMove(x, y);
}

bool UiDevice::touchUp(const int x, const int y)
{
    return mDeviceImpl->touchUp(x, y);
}

bool UiDevice::pressBack()
{
    return mDeviceImpl->pressBack();
}

bool UiDevice::pressHome()
{
    return mDeviceImpl->pressHome();
}

bool UiDevice::pressMenu()
{
    return mDeviceImpl->pressMenu();
}

bool UiDevice::pressVolUp()
{
    return mDeviceImpl->pressVolUp();
}

bool UiDevice::pressVolDown()
{
    return mDeviceImpl->pressVolDown();
}

bool UiDevice::pressPower()
{
    return mDeviceImpl->pressPower();
}

bool UiDevice::pressKeyCode(std::string keycode)
{
    return mDeviceImpl->pressKeyCode(keycode);
}

bool UiDevice::takeScreenshot(std::string path, float scale, int quality)
{
    return mDeviceImpl->takeScreenshot(path, scale, quality);
}