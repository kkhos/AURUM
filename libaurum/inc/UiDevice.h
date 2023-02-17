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
#include "TizenWindow.h"

#include "IDevice.h"
#include "ISearchable.h"

#include "Accessible.h"
#include "Waiter.h"

#include "Runnable.h"
#include "A11yEvent.h"

#include <functional>
#include <string>

#ifdef MQTT_ENABLED
#include "SaObject.h"
#include "ScreenAnalyzerWatcher.h"
#endif

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
     * @brief Simulates a press on the back key.
     *
     * @param[in] type one of @KeyRequestType
     *
     * @return true if the press back succeeded else false
     *
     * @since_tizen 6.5
     */
    bool pressBack(KeyRequestType type) override;

    /**
     * @brief Simulates a press on the home key.
     *
     * @param[in] type one of @KeyRequestType
     *
     * @return true if the press home succeeded else false
     *
     * @since_tizen 6.5
     */
    bool pressHome(KeyRequestType type) override;

    /**
     * @brief Simulates a press on the menu key.
     *
     * @param[in] type one of @KeyRequestType
     *
     * @return true if the press menu succeeded else false
     *
     * @since_tizen 6.5
     */
    bool pressMenu(KeyRequestType type) override;

    /**
     * @brief Simulates a press on the volume up key.
     *
     * @param[in] type one of @KeyRequestType
     *
     * @return true if the press volume up succeeded else false
     *
     * @since_tizen 6.5
     */
    bool pressVolUp(KeyRequestType type) override;

    /**
     * @brief Simulates a press on the volume down key.
     *
     * @param[in] type one of @KeyRequestType
     *
     * @return true if the press volume down succeeded else false
     *
     * @since_tizen 6.5
     */
    bool pressVolDown(KeyRequestType type) override;

    /**
     * @brief Simulates a press on the power key.
     *
     * @param[in] type one of @KeyRequestType
     *
     * @return true if the press power succeeded else false
     *
     * @since_tizen 6.5
     */
    bool pressPower(KeyRequestType type) override;

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
     * @param[in] scale scale the screenshot down if needed; 1.0f for original size
     * @param[in] quality quality of the PNG compression; range: 0-100
     *
     * @return true if screen shot is created successfully, false otherwise
     *
     * @since_tizen 6.5
     */
    bool takeScreenshot(std::string path, float scale, int quality) override;

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
     * @param[in] count count of render post evnet
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
    std::vector<std::shared_ptr<AccessibleNode>> getWindowRoot() const;

    /**
     * @brief Gets currently window information from window system.
     *
     * @return TizenWindow ptr vector
     *
     * @since_tizen 7.0
     */
    std::vector<std::shared_ptr<TizenWindow>> getTizenWindowInfo() const;

#ifdef MQTT_ENABLED
    /**
     * @brief Gets screen analyzer Object vector.
     *
     * @return SaObject ptr vector
     *
     * @since_tizen 7.0
     */
    std::vector<std::shared_ptr<SaObject>> getSaObject();

    /**
     * @brief Gets ScreenAnalyzerWatcher instance.
     *
     * @retrun ScreenAnalyzerWatcher ptr
     *
     * @since_tizen 7.0
     */
    std::shared_ptr<ScreenAnalyzerWatcher> getSAWatcher();
#endif
    /**
     * @brief Requests current screen analyze.
     *
     * @since_tizen 7.0
     */
    void RequestScreenAnalyze();

    /**
     * @brief Gets external application launched or not.
     *
     * @return true external application launched, otherwise false
     *
     * @since_tizen 7.0
     */
    bool getExternalAppLaunched();

    /**
     * @brief Sets whether to use screen analyzer for Tizen apps.
     *
     * @param[in] withScreenAnalyzer boolean value
     *
     * @since_tizen 7.0
     */
    void setWithScreenAnalyzer(bool withScreenAnalyzer);

    /**
     * @brief Gets whether to use screen analyzer for Tizen apps.
     *
     * @return true screen analyzer use for Tizen apps, otherwise false
     *
     * @since_tizen 7.0
     */
    bool getWithScreenAnalyzer();

private:
    /**
     * @brief Waits process idle.
     *
     * @since_tizen 6.5
     */
    bool waitForIdle() const;

private:
    /**
     * @brief UiDevice Consturctor.
     *
     * @since_tizen 6.5
     */
    UiDevice();

    /**
     * @brief UiDevice Consturctor with IDevice.
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
    static std::vector<std::shared_ptr<TizenWindow>> mTizenWindows;
    bool mIsWithSA;
#ifdef MQTT_ENABLED
    static std::shared_ptr<ScreenAnalyzerWatcher> mSAWatcher;
#endif
};

}

#endif
