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

#ifndef _UI_DEVICE_H_
#define _UI_DEVICE_H_

#include "config.h"

#include "UiObject.h"
#include "UiSelector.h"

#include "IDevice.h"
#include "ISearchable.h"

#include "Accessible.h"
#include "Waiter.h"

#include "Runnable.h"
#include "A11yEvent.h"

#include <functional>
#include <string>

namespace Aurum {

/**
 * @class UiDevice
 *
 * @ingroup aurum
 *
 * @brief UiDevice provides access to state information about the device.
 *        You can also use this class to simulate user actions on the device.
 */
class UiDevice : public IDevice, public ISearchable {
public:
    /**
     * @brief Performs a click at arbitrary coordinates specified by the user.
     *
     * @param[in] x x coordinate
     * @param[in] y y coordinate
     *
     * @return true if the click succeeded else false
     *
     * @since_tizen 6.5
     */
    bool click(const int x, const int y) override;

    /**
     * @brief Performs a click at arbitrary coordinates specified by the user during the specified time.
     *
     * @param[in] x x coordinate
     * @param[in] y y coordinate
     * @param[in] durationMs total time to maintain down action
     *
     * @return true if the click succeeded else false
     *
     * @since_tizen 6.5
     */
    bool click(const int x, const int y, const unsigned int durationMs) override;

    /**
     * @brief Performs a drag from one coordinate to another using the number of steps
     *        during the specified time.
     *
     * @param[in] sx start x coordinate
     * @param[in] sy start y coordinate
     * @param[in] ex end x coordinate
     * @param[in] ey end y coordinate
     * @param[in] steps the number of move steps
     * @param[in] durationMs total time to drag event
     *
     * @return true if the drag succeeded else false
     *
     * @since_tizen 6.5
     */
    bool drag(const int sx, const int sy, const int ex, const int ey,
              const int steps, const int durationMs) override;

    /**
     * @brief Performs a touch down at arbitrary coordinates specified by the user.
     *
     * @param[in] x x coordinate
     * @param[in] y y coordinate
     *
     * @return touch event number
     *
     * @since_tizen 6.5
     */
    int touchDown(const int x, const int y) override;

    /**
     * @brief Performs a touch move at arbitrary coordinates specified by the user.
     *
     * @param[in] x x coordinate
     * @param[in] y y coordinate
     * @param[in] seq touch event number
     *
     * @return true if the touch move succeeded else false
     *
     * @since_tizen 6.5
     */
    bool touchMove(const int x, const int y, const int seq) override;

    /**
     * @brief Performs a touch up at arbitrary coordinates specified by the user.
     *
     * @param[in] x x coordinate
     * @param[in] y y coordinate
     * @param[in] seq touch event number
     *
     * @return true if the touch up succeeded else false
     *
     * @since_tizen 6.5
     */
    bool touchUp(const int x, const int y, const int seq) override;

    /**
     * @brief Performs a wheel up on device with event amount and duration.
     *
     * @param[in] amount the number of wheel up event sent
     * @param[in] durationMs total time to wheel up event
     *
     * @return true if the wheel up succeeded else false
     *
     * @since_tizen 6.5
     */
    bool wheelUp(int amount, const int durationMs) override;

    /**
     * @brief Performs a wheel down on device with event amount and duration.
     *
     * @param[in] amount the number of wheel down event sent
     * @param[in] durationMs total time to wheel down event
     *
     * @return true if the wheel down succeeded else false
     *
     * @since_tizen 6.5
     */
    bool wheelDown(int amount, const int durationMs) override;

    /**
     * @brief Performs a mouse down at arbitrary coordinates specified by the user.
     *
     * @param[in] x x coordinate
     * @param[in] y y coordinate
     * @param[in] button mouse button number
     *
     * @return true if the mouse down succeeded else false
     *
     * @since_tizen 10.0
     */
    bool mouseDown(const int x, const int y, const int button) override;

    /**
     * @brief Performs a mouse move at arbitrary coordinates specified by the user.
     *
     * @param[in] x x coordinate
     * @param[in] y y coordinate
     * @param[in] button mouse button number
     *
     * @return true if the mouse move succeeded else false
     *
     * @since_tizen 10.0
     */
    bool mouseMove(const int x, const int y, const int button) override;

    /**
     * @brief Performs a mouse up at arbitrary coordinates specified by the user.
     *
     * @param[in] x x coordinate
     * @param[in] y y coordinate
     * @param[in] button mouse button number
     *
     * @return true if the mouse up succeeded else false
     *
     * @since_tizen 10.0
     */
    bool mouseUp(const int x, const int y, const int button) override;

    /**
     * @brief Simulates a press on the given keycode key.
     *
     * @param[in] keycode keycode
     * @param[in] type one of @KeyRequestType
     *
     * @return true if the press keycode succeeded else false
     *
     * @since_tizen 6.5
     */
    bool pressKeyCode(std::string keycode, KeyRequestType type) override;

    /**
     * @brief Simulates repeat press of the given keycode key.
     *
     * @param[in] keycode keycode
     * @param[in] intervalMs time interval for pressing given keycode key
     * @param[in] durationMs total time to press given keycode key
     *
     * @return true if the scroll keycode succeeded else false
     *
     * @since_tizen 7.0
     */
    bool repeatKeyCode(std::string keycode, int intervalMs, int durationMs) override;

    /**
     * @brief Take a screenshot of current window and store it as image file.
     *
     * @param[in] path where the image file should be written to
     * @param[in] asPixels asPixels wheter to receive screen pixel data
     * @param[in] pixels pixel pixel buffer data pointer
     * @param[in] angle rotation angle of current screen
     *
     * @return true if screen shot is created successfully, false otherwise
     *
     * @since_tizen 8.0
     */
    bool takeScreenshot(std::string path, bool asPixels, void **pixels, int angle = 0) override;

    /**
     * @brief Gets device system time.
     *
     * @param[in] type one of @TimeRequestType
     *
     * @return utc time stamp
     *
     * @since_tizen 6.5
     */
    long long getSystemTime(TimeRequestType type) override;

    /**
     * @brief Gets device screen size.
     *
     * @return @Size2D
     *
     * @since_tizen 6.5
     */
    const Size2D<int> getScreenSize() override;
public:

    /**
     * @brief Checks that there is object that satisfied with the selector condition in the device.
     *
     * @param[in] selector @UiSelector
     *
     * @return object exist or not
     *
     * @since_tizen 6.5
     */
    bool hasObject(const std::shared_ptr<UiSelector> selector) const override;

    /**
     * @brief Finds that object that satisfied with the selector condition in the device.
     *
     * @param[in] selector @UiSelector
     *
     * @return Found UiObject ptr
     *
     * @since_tizen 6.5
     */
    std::shared_ptr<UiObject> findObject(
        const std::shared_ptr<UiSelector> selector) const override;

    /**
     * @brief Finds that objects that satisfied with the selector condition in the device.
     *
     * @param[in] selector @UiSelector
     *
     * @return Found UiObject ptr vector
     *
     * @since_tizen 6.5
     */
    std::vector<std::shared_ptr<UiObject>> findObjects(
        const std::shared_ptr<UiSelector> selector) const override;

    /**
     * @copydoc ISearchable::getMatches()
     */
    std::vector<std::shared_ptr<UiObject>> getMatches(
        const std::shared_ptr<UiSelector> selector, const bool earlyReturn) const override;

    /**
     * @copydoc ISearchable::getMatchesInMatches()
     */
    std::vector<std::shared_ptr<UiObject>> getMatchesInMatches(
        const std::shared_ptr<UiSelector> firstSelector, const std::shared_ptr<UiSelector> secondSelector, const bool earlyReturn) const override;

    /**
     * TODO
     */
    bool waitFor(
        const std::function<bool(const ISearchable *)> condition) const;

    /**
     * TODO
     */
    std::shared_ptr<UiObject> waitFor(
        const std::function<std::shared_ptr<UiObject>(const ISearchable *)>
            condition) const;

    /**
     * @brief Waits for a specific A11y Event for a some time by user set.
     *
     * @param[in] type one of @A11yEvent
     * @param[in] timeout time(millisecond) to wait event occur
     *
     * @return true if event happened in time, otherwise false
     *
     * @since_tizen 6.5
     */
    bool waitForEvents(
        const A11yEvent type, const int timeout) const;

    /**
     * @brief Sends key event and Wait for a specific A11y Event for a some time by user set.
     *
     * @param[in] keycode keycode
     * @param[in] type one of @A11yEvent
     * @param[in] timeout time(millisecond) to wait event occur
     *
     * @return true if event happened in time, otherwise false
     *
     * @since_tizen 6.5
     */
    bool sendKeyAndWaitForEvents(
        const std::string keycode, const A11yEvent type, const int timeout) const;

    /**
     * @brief Runs command and Wait for a specific A11y Event for a some time by user set.
     *
     * @param[in] cmd @Runnable
     * @param[in] type one of @A11yEvent
     * @param[in] timeout time(millisecond) to wait event occur
     * @param[in] packageName package name that event occurs
     * @param[in] count count of render post event
     *
     * @return true if event happened in time, otherwise false
     *
     * @since_tizen 6.5
     */
    bool executeAndWaitForEvents(
        const Runnable *cmd, const A11yEvent type, const int timeout, const std::string packageName, const int count) const;

/**
     * @brief Register atspi event callback.
     *
     * @param[in] type @A11yEvent
     * @param[in] cb @EventHandler
     * @param[in] data @void pointer
     *
     * @return true if callback is registered, else false
     * @since_tizen 7.0
     */
    bool registerCallback(const A11yEvent type, EventHandler cb, void *data) const;

public:
    /**
     * @brief Gets UiDevice instance.
     *
     * @return UiDevice ptr
     *
     * @since_tizen 6.5
     */
    static std::shared_ptr<UiDevice> getInstance(IDevice *deviceImpl = nullptr);

    /**
     * @brief Gets currently enabled applications root window.
     *
     * @return AccessibleNode ptr vector
     *
     * @since_tizen 6.5
     */
    std::vector<std::shared_ptr<AccessibleNode>> getWindowRoot() const override;

    /**
     * @brief Gets external application launched or not.
     *
     * @return true external application launched, otherwise false
     *
     * @since_tizen 7.0
     */
    bool getExternalAppLaunched();

    /**
     * @brief Gets angle of root window.
     *
     * @return window angle int
     *
     * @since_tizen 7.5
     */
    int getWindowAngle();

        /**
     * @brief Gets angle of target device.
     *
     * @return target angle int
     *
     * @since_tizen 7.5
     */
    int getTargetAngle();

    /**
     * @brief Simulates a press on the key.
     *
     * @param[in] keyType one of @KeyType
     * @param[in] keyReqestType one of @KeyRequestType
     *
     * @return true if the press key succeeded else false
     *
     * @since_tizen 8.0
     */
    bool generateKey(KeyType keyType, KeyRequestType keyReqestType);

    /**
     * @brief Simulates a hardware key.
     *
     * @param[in] type one of @KeyRequestType
     *
     * @return true if the press back succeeded else false
     *
     * @deprecated Since 9.0
     */
    bool pressBack(KeyRequestType type);
    bool pressHome(KeyRequestType type);
    bool pressMenu(KeyRequestType type);
    bool pressVolUp(KeyRequestType type);
    bool pressVolDown(KeyRequestType type);
    bool pressPower(KeyRequestType type);

private:
    /**
     * @brief Waits process idle.
     *
     * @since_tizen 6.5
     */
    bool waitForIdle() const;

private:
    /**
     * @brief UiDevice Constructor.
     *
     * @since_tizen 6.5
     */
    UiDevice();

    /**
     * @brief UiDevice Constructor with IDevice.
     *
     * @since_tizen 6.5
     */
    UiDevice(IDevice *impl);

public:
    /**
     * @brief UiDevice Destructor.
     *
     * @since_tizen 6.5
     */
    virtual ~UiDevice();

private:
    IDevice *mDeviceImpl;
    const Waiter *mWaiter;
    static std::once_flag mOnceFlag;
};

}

#endif
