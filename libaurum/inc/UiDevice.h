#pragma once

#include "config.h"

#include "UiObject.h"
#include "UiSelector.h"

#include "IDevice.h"
#include "ISearchable.h"

#include "AccessibleNode.h"
#include "Waiter.h"

#include "Runnable.h"
#include "A11yEvent.h"

#include <functional>
#include <string>

/**
 * @brief UiDevice class
 * @since_tizen 5.5
 */
class UiDevice : public IDevice, public ISearchable {
public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool click(const int x, const int y) override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool click(const int x, const int y, const unsigned int intv) override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool drag(const int sx, const int sy, const int ex, const int ey,
              const int steps, const int durationMs) override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    int touchDown(const int x, const int y) override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool touchMove(const int x, const int y, const int seq) override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool touchUp(const int x, const int y, const int seq) override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool wheelUp(int amount, const int durationMs) override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool wheelDown(int amount, const int durationMs) override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool pressBack(KeyRequestType type) override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool pressHome(KeyRequestType type) override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool pressMenu(KeyRequestType type) override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool pressVolUp(KeyRequestType type) override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool pressVolDown(KeyRequestType type) override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool pressPower(KeyRequestType type) override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool pressKeyCode(std::string keycode, KeyRequestType type) override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool takeScreenshot(std::string path, float scale, int quality) override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    long long getSystemTime(TimeRequestType type) override;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    const Rect<int> getScreenSize() override;
public:

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool hasObject(const std::shared_ptr<UiSelector> selector) const override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::shared_ptr<UiObject> findObject(
        const std::shared_ptr<UiSelector> selector) const override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::vector<std::shared_ptr<UiObject>> findObjects(
        const std::shared_ptr<UiSelector> selector) const override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool waitFor(
        const std::function<bool(const ISearchable *)> condition) const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::shared_ptr<UiObject> waitFor(
        const std::function<std::shared_ptr<UiObject>(const ISearchable *)>
            condition) const;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    bool waitForEvents(
        const A11yEvent type, const int timeout) const;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    bool sendKeyAndWaitForEvents(
        const std::string keycode, const A11yEvent type, const int timeout) const;

    bool executeAndWaitForEvents(
        const Runnable *cmd, const A11yEvent type, const int timeout) const;
public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    static std::shared_ptr<UiDevice> getInstance(IDevice *deviceImpl = nullptr);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::vector<std::shared_ptr<AccessibleNode>> getWindowRoot() const;
private:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool waitForIdle() const;

private:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiDevice();

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiDevice(IDevice *impl);

public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual ~UiDevice();

private:
    /**
     * @brief TBD
     */
    IDevice *mDeviceImpl;
    /**
     * @brief TBD
     */
    const Waiter *mWaiter;
};
