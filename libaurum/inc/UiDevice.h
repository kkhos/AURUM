#ifndef UIDEVICE_H
#define UIDEVICE_H
#include "config.h"

#include "UiObject.h"
#include "UiSelector.h"

#include "IDevice.h"
#include "ISearchable.h"

#include "AccessibleNode.h"
#include "Waiter.h"

#include <functional>
#include <string>

enum class DeviceType {
    DEFAULT,
    NUM_DEVICETYPE,
};

class UiDevice : public IDevice, public ISearchable {
public:
    bool click(const int x, const int y) override;
    bool click(const int x, const int y, const unsigned int intv) override;

    bool drag(const int sx, const int sy, const int ex, const int ey,
              const int steps, const int durationMs) override;

    bool touchDown(const int x, const int y) override;
    bool touchMove(const int x, const int y) override;
    bool touchUp(const int x, const int y) override;

    bool wheelUp(int amount, const int durationMs) override;
    bool wheelDown(int amount, const int durationMs) override;

    bool pressBack() override;
    bool pressHome() override;
    bool pressMenu() override;
    bool pressVolUp() override;
    bool pressVolDown() override;
    bool pressPower() override;

    bool pressKeyCode(std::string keycode) override;
    bool takeScreenshot(std::string path, float scale, int quality) override;
    long long getSystemTime(TypeRequestType type) override;

public:
    bool hasObject(const std::shared_ptr<UiSelector> selector) const override;
    std::unique_ptr<UiObject> findObject(
        const std::shared_ptr<UiSelector> selector) const override;
    std::vector<std::unique_ptr<UiObject>> findObjects(
        const std::shared_ptr<UiSelector> selector) const override;

    bool waitFor(
        const std::function<bool(const ISearchable *)> condition) const;
    std::unique_ptr<UiObject> waitFor(
        const std::function<std::unique_ptr<UiObject>(const ISearchable *)>
            condition) const;

public:
    static UiDevice *getInstance(DeviceType type);

private:
    std::vector<std::unique_ptr<AccessibleNode>> getWindowRoot() const;
    bool waitForIdle() const;
private:
    UiDevice();
    UiDevice(DeviceType type, IDevice *impl);
    virtual ~UiDevice();

private:
    DeviceType    mType;
    IDevice *     mDeviceImpl;
    const Waiter *mWaiter;
};

#endif