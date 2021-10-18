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

#ifndef _IDEVICE_H_
#define _IDEVICE_H_

#include "config.h"
#include "Rect.h"
#include "Size2D.h"
#include <string>

namespace Aurum {

/**
 * @brief TimeRequestType enum class.
 *
 * @since_tizen 5.5
 */
enum class TimeRequestType {
    WALLCLOCK, //Real time of system
    MONOTONIC, //monotonic time of system
};

/**
 * @brief KeyRequestType enum class.
 *
 * @since_tizen 5.5
 */
enum class KeyRequestType {
    STROKE,      //Key press(100ms) and release
    LONG_STROKE, //Key long press(2000ms) and release
    PRESS,       //Key press
    RELEASE,     //Key release
};

/**
 * @brief IDevice interface.
 *        It defines common device controls such as touch, key events
 *
 * @since_tizen 5.5
 */
class IDevice {
public:
    /**
     * @brief IDevice Destructor.
     *
     * @since_tizen 5.5
     */
    virtual ~IDevice() {}

    /**
     * @brief Performs a click at arbitrary coordinates specified by the user.
     *
     * @param x x coordinate
     * @param y y coordinate
     *
     * @return true if the click succeeded else false
     *
     * @since_tizen 5.5
     */
    virtual bool click(const int x, const int y) = 0;

    /**
     * @brief Performs a click at arbitrary coordinates specified by the user during the specified time.
     *
     * @param x x coordinate
     * @param y y coordinate
     * @param durationMs total time to maintain down action
     *
     * @return true if the click succeeded else false
     *
     * @since_tizen 5.5
     */
    virtual bool click(const int x, const int y, const unsigned int durationMs) = 0;

    /**
     * @brief Performs a drag from one coordinate to another using the number of steps
     *        during the specified time.
     *
     * @param sx start x coordinate
     * @param sy start y coordinate
     * @param ex end x coordinate
     * @param ey end y coordinate
     * @param steps the number of move steps
     * @param durationMs total time to drag event
     *
     * @return true if the drag succeeded else false
     *
     * @since_tizen 5.5
     */
    virtual bool drag(const int sx, const int sy, const int ex, const int ey,
                      const int steps, const int durationMs) = 0;

    /**
     * @brief Performs a touch down at arbitrary coordinates specified by the user.
     *
     * @param x x coordinate
     * @param y y coordinate
     *
     * @return touch event number
     *
     * @since_tizen 5.5
     */
    virtual int touchDown(const int x, const int y) = 0;

    /**
     * @brief Performs a touch move at arbitrary coordinates specified by the user.
     *
     * @param x x coordinate
     * @param y y coordinate
     * @param seq touch event number
     *
     * @return true if the touch move succeeded else false
     *
     * @since_tizen 5.5
     */
    virtual bool touchMove(const int x, const int y, const int seq) = 0;

    /**
     * @brief Performs a touch up at arbitrary coordinates specified by the user.
     *
     * @param x x coordinate
     * @param y y coordinate
     * @param seq touch event number
     *
     * @return true if the touch up succeeded else false
     *
     * @since_tizen 5.5
     */
    virtual bool touchUp(const int x, const int y, const int seq) = 0;

    /**
     * @brief Performs a wheel up on device with event amount and duration.
     *
     * @param amount the number of wheel up event sent
     * @param durationMs total time to wheel up event
     *
     * @return true if the wheel up succeeded else false
     *
     * @since_tizen 5.5
     */
    virtual bool wheelUp(int amount, const int durationMs) = 0;

    /**
     * @brief Performs a wheel down on device with event amount and duration.
     *
     * @param amount the number of wheel down event sent
     * @param durationMs total time to wheel down event
     *
     * @return true if the wheel down succeeded else false
     *
     * @since_tizen 5.5
     */
    virtual bool wheelDown(int amount, const int durationMs) = 0;

    /**
     * @brief Simulates a press on the back key.
     *
     * @param type one of @KeyRequestType
     *
     * @return true if the press back succeeded else false
     *
     * @since_tizen 5.5
     */
    virtual bool pressBack(KeyRequestType type) = 0;

    /**
     * @brief Simulates a press on the home key.
     *
     * @param type one of @KeyRequestType
     *
     * @return true if the press home succeeded else false
     *
     * @since_tizen 5.5
     */
    virtual bool pressHome(KeyRequestType type) = 0;

    /**
     * @brief Simulates a press on the menu key.
     *
     * @param type one of @KeyRequestType
     *
     * @return true if the press menu succeeded else false
     *
     * @since_tizen 5.5
     */
    virtual bool pressMenu(KeyRequestType type) = 0;

    /**
     * @brief Simulates a press on the volume up key.
     *
     * @param type one of @KeyRequestType
     *
     * @return true if the press volume up succeeded else false
     *
     * @since_tizen 5.5
     */
    virtual bool pressVolUp(KeyRequestType type) = 0;

    /**
     * @brief Simulates a press on the volume down key.
     *
     * @param type one of @KeyRequestType
     *
     * @return true if the press volume down succeeded else false
     *
     * @since_tizen 5.5
     */
    virtual bool pressVolDown(KeyRequestType type) = 0;

    /**
     * @brief Simulates a press on the power key.
     *
     * @param type one of @KeyRequestType
     *
     * @return true if the press power succeeded else false
     *
     * @since_tizen 5.5
     */
    virtual bool pressPower(KeyRequestType type) = 0;

    /**
     * @brief Simulates a press on the given keycode key.
     *
     * @param keycode keycode
     * @param type one of @KeyRequestType
     *
     * @return true if the press keycode succeeded else false
     *
     * @since_tizen 5.5
     */
    virtual bool pressKeyCode(std::string keycode, KeyRequestType type) = 0;

    /**
     * @brief Take a screenshot of current window and store it as image file.
     *
     * @param path where the image file should be written to
     * @param scale scale the screenshot down if needed; 1.0f for original size
     * @param quality quality of the PNG compression; range: 0-100
     *
     * @return true if screen shot is created successfully, false otherwise
     *
     * @since_tizen 5.5
     */
    virtual bool takeScreenshot(std::string path, float scale, int quality) = 0;

    /**
     * @brief Gets device system time.
     *
     * @param type one of @TimeRequestType
     *
     * @return utc time stamp
     *
     * @since_tizen 5.5
     */
    virtual long long getSystemTime(TimeRequestType type) = 0;

    /**
     * @brief Gets device screen size.
     *
     * @return @Size2D
     *
     * @since_tizen 6.5
     */
    virtual const Size2D<int> getScreenSize() = 0;
};

}

#endif
