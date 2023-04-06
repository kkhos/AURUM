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

#ifndef _MOCK_DEVICE_IMPL_H_
#define _MOCK_DEVICE_IMPL_H_

#include "config.h"
#include "AccessibleNode.h"
#include "IDevice.h"

#include <set>
#include <list>
#include <tuple>
#include <vector>
#include <map>

using namespace Aurum;

namespace AurumInternal {

namespace Mock {

enum class MockKeyType {
    BACK,
    HOME,
    MENU,
    VOLUP,
    VOLDOWN,
    POWER,
    KEY
};

struct TouchData { int x; int y; long long stamp1; long long stamp2;};
class MockDeviceImpl : public IDevice {
public:
    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    MockDeviceImpl();

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    ~MockDeviceImpl();

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    bool click(const int x, const int y) override;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    bool click(const int x, const int y, const unsigned int durationMs) override;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    bool drag(const int sx, const int sy, const int ex, const int ey,
              const int steps, const int durationMs) override;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    int touchDown(const int x, const int y) override;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    bool touchMove(const int x, const int y, const int seq) override;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    bool touchUp(const int x, const int y, const int seq) override;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    bool wheelUp(int amount, const int durationMs) override;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    bool wheelDown(int amount, const int durationMs) override;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    bool pressBack(KeyRequestType type) override;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    bool pressHome(KeyRequestType type) override;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    bool pressMenu(KeyRequestType type) override;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    bool pressVolUp(KeyRequestType type) override;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    bool pressVolDown(KeyRequestType type) override;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    bool pressPower(KeyRequestType type) override;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    bool pressKeyCode(std::string keycode, KeyRequestType type) override;

     /**
     * @brief TBD
     * @since_tizen 7.0
     */
    bool repeatKeyCode(std::string keycode, int intervalMs, int durationMs) override;

    /**
     * @brief TBD
     * @since_tizen 7.0
     */
    bool takeScreenshot(std::string path, bool asPixels, void **pixels) override;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    long long getSystemTime(TimeRequestType type) override;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    const Size2D<int> getScreenSize() override;

    /**
     * @copydoc IDevice::getWindowRoot()
     */
    std::vector<std::shared_ptr<AccessibleNode>> getWindowRoot() const override;

protected:
    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    bool strokeKeyCode(std::string keycode, unsigned int durationMs);

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    bool pressKeyCode(std::string keycode);

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    bool releaseKeyCode(std::string keycode);

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    int grabTouchSeqNumber();

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    bool releaseTouchSeqNumber(int seq);

private:
    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    void startTimer(void);

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    int stopTimer(void);

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    long long timeStamp(void);

public:
    /**
     * @brief TBD
     */
    static const int INTV_CLICK = 5;

    /**
     * @brief TBD
     */
    static const int INTV_SHORTSTROKE = 100;

    /**
     * @brief TBD
     */
    static const int INTV_LONGSTROKE = 2000;

    /**
     * @brief TBD
     */
    static const int INTV_MINIMUM_DRAG_MS = 25;

    /**
     * @brief TBD
     */
    static const int INTV_MINIMUM_USLEEP = 1000;

    /**
     * @brief TBD
     */
    static const int MINIMUM_DURATION_DRAG = 100;

    /**
     * @brief TBD
     */
    static const unsigned int MSEC_PER_SEC = 1000;

    /**
     * @brief TBD
     */
    static const unsigned int MAX_FINGER_NUMBER = 2;

    /**
     * @brief TBD
     */
    struct timespec tStart;

    /**
     * @brief TBD
     */
    bool isTimerStarted;

    /**
     * @brief TBD
     */
    std::set<int> mTouchSeq;

public:
    /**
     * @brief TBD
     */
    std::map<int, TouchData> mTouchRelease;

    /**
     * @brief TBD
     */
    std::map<int, TouchData> mTouchDown;

    /**
     * @brief TBD
     */
    std::vector<std::tuple<MockKeyType, KeyRequestType, std::string>> mKeyDevice;

    /**
     * @brief TBD
     */
    int mWheelDevice;

    /**
     * @brief TBD
     */
    Size2D<int> mScreenSize;
};

}

}

#endif
