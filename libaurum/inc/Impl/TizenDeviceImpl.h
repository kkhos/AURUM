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

#ifndef _TIZEN_DEVICE_IMPL_H_
#define _TIZEN_DEVICE_IMPL_H_

#include "config.h"
#include "IDevice.h"
#include "TizenWindow.h"

#include <set>
#include <efl_util.h>
#include <efl_util_screenshot_extension.h>
#include <shared_mutex>

using namespace Aurum;

namespace AurumInternal {

class TizenDeviceImpl : public IDevice {
public:
    TizenDeviceImpl();

    ~TizenDeviceImpl();

    /**
     * @copydoc IDevice::click()
     */
    bool click(const int x, const int y) override;

    /**
     * @copydoc IDevice::click()
     */
    bool click(const int x, const int y, const unsigned int durationMs) override;

    /**
     * @copydoc IDevice::drag()
     */
    bool drag(const int sx, const int sy, const int ex, const int ey,
              const int steps, const int durationMs) override;

    /**
     * @copydoc IDevice::touchDown()
     */
    int touchDown(const int x, const int y) override;

    /**
     * @copydoc IDevice::touchMove()
     */
    bool touchMove(const int x, const int y, const int seq) override;

    /**
     * @copydoc IDevice::touchUp()
     */
    bool touchUp(const int x, const int y, const int seq) override;

    /**
     * @copydoc IDevice::wheelUp()
     */
    bool wheelUp(int amount, const int durationMs) override;

    /**
     * @copydoc IDevice::wheelDown()
     */
    bool wheelDown(int amount, const int durationMs) override;

    /**
     * @copydoc IDevice::pressKeyCode()
     */
    bool pressKeyCode(std::string keycode, KeyRequestType type) override;

    /**
     * @copydoc IDevice::repeatKeyCode()
     */
    bool repeatKeyCode(std::string keycode, int intervalMs, int durationMs) override;

    /**
     * @copydoc IDevice::takeScreenshot()
     */
    bool takeScreenshot(std::string path, bool asPixels, void **pixels, int angle = 0) override;

    /**
     * @copydoc IDevice::getSystemTime()
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

    /**
     * @copydoc IDevice::getWindowRoot()
     */
    std::vector<std::shared_ptr<AccessibleNode>> getWindowRoot() const override;

protected:
    /**
     * @brief Press and release given key during duration time.
     *
     * @since_tizen 6.5
     */
    bool strokeKeyCode(std::string keycode, unsigned int durationMs);

    /**
     * @brief Press given key.
     *
     * @since_tizen 6.5
     */
    bool pressKeyCode(std::string keycode);

    /**
     * @brief Release given key.
     *
     * @since_tizen 6.5
     */
    bool releaseKeyCode(std::string keycode);

    /**
     * @brief Increase touch count and return the number to manage touch count.
     *
     * @since_tizen 6.5
     */
    int grabTouchSeqNumber();

    /**
     * @brief Delete given touch number.
     *
     * @since_tizen 6.5
     */
    bool releaseTouchSeqNumber(int seq);

private:
    /**
     * @brief Timer utility.
     *
     * @since_tizen 6.5
     */
    void startTimer(void);

    /**
     * @brief Timer utility.
     *
     * @since_tizen 6.5
     */
    int stopTimer(void);

    /**
     * @brief Gets currently window information from window system.
     *
     * @return TizenWindow ptr vector
     *
     * @since_tizen 7.0
     */
    void getTizenWindowInfo(std::vector<std::shared_ptr<TizenWindow>> &mTizenWindows) const;

private:
    efl_util_inputgen_h mFakeTouchHandle;
    efl_util_inputgen_h mFakeKeyboardHandle;
    efl_util_inputgen_h mFakeWheelHandle;
    static const int INTV_CLICK = 5;
    static const int INTV_SHORTSTROKE = 10;
    static const int INTV_LONGSTROKE = 2000;
    static const int INTV_MINIMUM_DRAG_MS = 25;
    static const int INTV_MINIMUM_USLEEP = 1000;
    static const int MINIMUM_DURATION_DRAG = 100;
    static const int MINIMUM_REPEAT_INTERVAL = 20;
    static const unsigned int MSEC_PER_SEC = 1000;
    static const unsigned int USEC_PER_MSEC = 1000;
    static const unsigned int MAX_FINGER_NUMBER = 2;
    struct timespec tStart;
    bool isTimerStarted;
    std::set<int> mTouchSeq;

    /**
     * @brief TBD
     */
    Size2D<int> mScreenSize;
    static std::mutex CaptureMutex;
    static std::vector<std::shared_ptr<AccessibleNode>> mCachedNode;
};

}

#endif
