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

#include "TizenDeviceImpl.h"

#include <functional>
#include <tuple>
#include <iostream>
#include <memory>

#include <stdlib.h>
#include <time.h>
#include <Ecore.h>

#include <tdm_helper.h>
#include <tbm_surface.h>
#include <system_info.h>

using namespace Aurum;
using namespace AurumInternal;

#define NANO_SEC 1000000000.0
#define MICRO_SEC 1000000


std::mutex TizenDeviceImpl::CaptureMutex = std::mutex{};

TizenDeviceImpl::TizenDeviceImpl()
: mFakeTouchHandle{0}, mFakeKeyboardHandle{0}, mFakeWheelHandle{0}, tStart{}, isTimerStarted{false}, mTouchSeq{}
{
    LOGI("device implementation init");
    TizenDeviceImpl *obj = static_cast<TizenDeviceImpl *>(this);
    obj->mFakeTouchHandle = efl_util_input_initialize_generator_with_sync(EFL_UTIL_INPUT_DEVTYPE_TOUCHSCREEN, NULL);
    obj->mFakeKeyboardHandle =
       efl_util_input_initialize_generator_with_sync(EFL_UTIL_INPUT_DEVTYPE_KEYBOARD, NULL);

    int width = 0;
    int height = 0;
    system_info_get_platform_int("http://tizen.org/feature/screen.width", &width);
    system_info_get_platform_int("http://tizen.org/feature/screen.height", &height);

    mScreenSize = Size2D<int>{width, height};
}

TizenDeviceImpl::~TizenDeviceImpl()
{
   TizenDeviceImpl *obj = static_cast<TizenDeviceImpl *>(this);
   efl_util_input_deinitialize_generator(obj->mFakeTouchHandle);
   efl_util_input_deinitialize_generator(obj->mFakeKeyboardHandle);
}

bool TizenDeviceImpl::click(const int x, const int y)
{
    return click(x, y, INTV_CLICK);
}

bool TizenDeviceImpl::click(const int x, const int y, const unsigned int durationMs)
{
    LOGI("click %d %d , durationMs:%d", x, y, durationMs);

    int seq = touchDown(x, y);
    if (seq < 0) return false;
    usleep(durationMs * MSEC_PER_SEC);
    touchUp(x, y, seq);

    return true;
}


int TizenDeviceImpl::touchDown(const int x, const int y)
{
    int seq = grabTouchSeqNumber();
    LOGI("touch down %d %d , seq:%d", x, y, seq);
    if (seq >= 0) {
         TizenDeviceImpl *obj = static_cast<TizenDeviceImpl *>(this);
         long result = (long)efl_util_input_generate_touch(obj->mFakeTouchHandle, seq, EFL_UTIL_INPUT_TOUCH_BEGIN,
                                                           x, y);

         if (result != EFL_UTIL_ERROR_NONE) {
              releaseTouchSeqNumber(seq);
              return -1;
         }
    }
    return seq;
}

bool TizenDeviceImpl::touchMove(const int x, const int y, const int seq)
{
    LOGI("touch move %d %d, seq:%d", x, y, seq);
    if (seq >= 0) {
         TizenDeviceImpl *obj = static_cast<TizenDeviceImpl *>(this);
         long result = (long)efl_util_input_generate_touch(obj->mFakeTouchHandle, seq, EFL_UTIL_INPUT_TOUCH_UPDATE,
                                                           x, y);
         return result == EFL_UTIL_ERROR_NONE;
    }
    return false;
}

bool TizenDeviceImpl::touchUp(const int x, const int y, const int seq)
{
   LOGI("touch up %d %d, seq:%d", x, y, seq);
   if (seq >= 0) {
        TizenDeviceImpl *obj = static_cast<TizenDeviceImpl *>(this);
        long result = (long)efl_util_input_generate_touch(obj->mFakeTouchHandle, seq, EFL_UTIL_INPUT_TOUCH_END,
                                                          x, y);
        return releaseTouchSeqNumber(seq) && result == EFL_UTIL_ERROR_NONE;
    }
    return false;
}

bool TizenDeviceImpl::wheelUp(int amount, const int durationMs)
{
    LOGI("wheel up %d for %d", amount, durationMs);
    long result = -1;
    TizenDeviceImpl *obj = static_cast<TizenDeviceImpl *>(this);
    obj->mFakeWheelHandle = efl_util_input_initialize_generator_with_sync(EFL_UTIL_INPUT_DEVTYPE_POINTER, NULL);
    for (int i = 0; i < amount; i++){
         TizenDeviceImpl *obj = static_cast<TizenDeviceImpl *>(this);
         result = (long)efl_util_input_generate_wheel(obj->mFakeWheelHandle, EFL_UTIL_INPUT_POINTER_WHEEL_HORZ, 1);
         usleep(durationMs * MSEC_PER_SEC/amount);
    }
    efl_util_input_deinitialize_generator(obj->mFakeWheelHandle);

    return result == EFL_UTIL_ERROR_NONE;
}

bool TizenDeviceImpl::wheelDown(int amount, const int durationMs)
{
    LOGI("wheel down %d for %d", amount, durationMs);
    long result = -1;
    TizenDeviceImpl *obj = static_cast<TizenDeviceImpl *>(this);
    obj->mFakeWheelHandle = efl_util_input_initialize_generator_with_sync(EFL_UTIL_INPUT_DEVTYPE_POINTER, NULL);
    for (int i = 0; i < amount; i++){
        TizenDeviceImpl *obj = static_cast<TizenDeviceImpl *>(this);
        result = (long)efl_util_input_generate_wheel(obj->mFakeWheelHandle, EFL_UTIL_INPUT_POINTER_WHEEL_HORZ, -1);
        usleep(durationMs * MSEC_PER_SEC/amount);
    }
    efl_util_input_deinitialize_generator(obj->mFakeWheelHandle);

    return result == EFL_UTIL_ERROR_NONE;
}
void TizenDeviceImpl::startTimer(void)
{
    isTimerStarted = true;
    clock_gettime(CLOCK_MONOTONIC, &tStart);
}

int TizenDeviceImpl::stopTimer(void)
{
    struct timespec tEnd;
    if (!isTimerStarted) return 0;
    isTimerStarted = false;
    clock_gettime(CLOCK_MONOTONIC, &tEnd);
    return ((tEnd.tv_sec + tEnd.tv_nsec/NANO_SEC) - (tStart.tv_sec + tStart.tv_nsec/NANO_SEC)) * MICRO_SEC;
}

bool TizenDeviceImpl::drag(const int sx, const int sy, const int ex, const int ey,
                         const int steps, const int durationMs)
{
    unsigned int _stepUs;
    int _durationUs, _steps;
    int consumptionUs;

    if (steps <= 0) _steps = 1;
    else _steps = steps;

    if (durationMs < MINIMUM_DURATION_DRAG) _durationUs = INTV_MINIMUM_DRAG_MS * MSEC_PER_SEC;
    else _durationUs = durationMs * MSEC_PER_SEC;

    _stepUs = _durationUs / (_steps +1);

    if (_stepUs < (INTV_MINIMUM_DRAG_MS * MSEC_PER_SEC)) {
        _steps = (int)(_durationUs / (INTV_MINIMUM_DRAG_MS * MSEC_PER_SEC)) - 1;
        _stepUs  = (INTV_MINIMUM_DRAG_MS * MSEC_PER_SEC);
    }
    LOGI("flicking (%d, %d) -> (%d, %d) for (%d ms)", sx, sy, ex, ey, durationMs);
    startTimer();
    int seq = touchDown(sx, sy);
    if (seq < 0) return false;
    consumptionUs = stopTimer();

    for ( int s = 1; s <= _steps + 1; s++) {
        usleep((_stepUs - consumptionUs)>INTV_MINIMUM_USLEEP?(_stepUs - consumptionUs):INTV_MINIMUM_USLEEP);
        startTimer();
        touchMove(sx + (ex - sx) * s / (steps + 1), sy + (ey - sy) * s / (steps + 1), seq);
        consumptionUs = stopTimer();
    }
    usleep((_stepUs - consumptionUs)>INTV_MINIMUM_USLEEP?(_stepUs - consumptionUs):INTV_MINIMUM_USLEEP);
    touchMove(ex, ey, seq);
    touchUp(ex, ey, seq);

    return true;
}

bool TizenDeviceImpl::pressBack(KeyRequestType type)
{
    return pressKeyCode("XF86Back", type);
}

bool TizenDeviceImpl::pressHome(KeyRequestType type)
{
    return pressKeyCode("XF86Home", type);
}

bool TizenDeviceImpl::pressMenu(KeyRequestType type)
{
    return pressKeyCode("XF86Menu", type);
}

bool TizenDeviceImpl::pressVolUp(KeyRequestType type)
{
    return pressKeyCode("XF86AudioRaiseVolume", type);
}

bool TizenDeviceImpl::pressVolDown(KeyRequestType type)
{
    return pressKeyCode("XF86AudioLowerVolume", type);
}

bool TizenDeviceImpl::pressPower(KeyRequestType type)
{
    return pressKeyCode("XF86PowerOff", type);
}

bool TizenDeviceImpl::pressKeyCode(std::string keycode, KeyRequestType type)
{
    if (type == KeyRequestType::STROKE)
        return strokeKeyCode(keycode, INTV_SHORTSTROKE);
    else if (type == KeyRequestType::LONG_STROKE)
        return strokeKeyCode(keycode, INTV_LONGSTROKE);
    else if (type == KeyRequestType::PRESS)
        return pressKeyCode(keycode);
    else if (type == KeyRequestType::RELEASE)
        return releaseKeyCode(keycode);
    else if (type == KeyRequestType::REPEAT)
    {
        LOGI("You can't repeat non-XF86 keys");
        return false;
    }

    return false;
}

bool TizenDeviceImpl::repeatKeyCode(std::string keycode, int intervalMs, int durationMs)
{
    if (intervalMs < MINIMUM_REPEAT_INTERVAL)
    {
        LOGI("Minimum intervalMs is %d, but user has set it to %d, so changed it to %d", MINIMUM_REPEAT_INTERVAL, intervalMs, MINIMUM_REPEAT_INTERVAL);
        intervalMs = MINIMUM_REPEAT_INTERVAL;
    }

    int press_count = durationMs / intervalMs;

    for (int i = 0; i < press_count; i++)
    {
        strokeKeyCode(keycode, INTV_SHORTSTROKE);
        usleep((intervalMs - INTV_SHORTSTROKE) * USEC_PER_MSEC);
    }
    strokeKeyCode(keycode, INTV_SHORTSTROKE);

    return true;
}

bool TizenDeviceImpl::strokeKeyCode(std::string keycode, unsigned int durationMs)
{
    pressKeyCode(keycode);
    usleep(durationMs * USEC_PER_MSEC);
    releaseKeyCode(keycode);
    return true;
}

bool TizenDeviceImpl::pressKeyCode(std::string keycode)
{
   TizenDeviceImpl *obj = static_cast<TizenDeviceImpl *>(this);
   long result = (long)efl_util_input_generate_key(obj->mFakeKeyboardHandle, keycode.c_str(), 1);
   return result == EFL_UTIL_ERROR_NONE;
}

bool TizenDeviceImpl::releaseKeyCode(std::string keycode)
{
   TizenDeviceImpl *obj = static_cast<TizenDeviceImpl *>(this);
    long result = (long)efl_util_input_generate_key(obj->mFakeKeyboardHandle, keycode.c_str(), 0);
    return result == EFL_UTIL_ERROR_NONE;
}

bool TizenDeviceImpl::takeScreenshot(std::string path, float scale, int quality)
{
    efl_util_screenshot_h screenshot = NULL;
    tbm_surface_h tbm_surface = NULL;

    CaptureMutex.lock();
    screenshot = efl_util_screenshot_initialize(mScreenSize.width, mScreenSize.height);

    if (screenshot) {
        tbm_surface = efl_util_screenshot_take_tbm_surface(screenshot);
        if (tbm_surface) {
            tdm_helper_dump_buffer(tbm_surface, path.c_str());
            sync();
            tbm_surface_destroy(tbm_surface);
        } else {
            efl_util_screenshot_deinitialize(screenshot);
            CaptureMutex.unlock();
            return false;
        }
        efl_util_screenshot_deinitialize(screenshot);
    } else {
        CaptureMutex.unlock();
        return false;
    }
    CaptureMutex.unlock();
    return true;
}

class Clock {
public:
    virtual ~Clock(){};

public:
    virtual long long getTime() = 0;
protected:
    long long convertTime(struct timespec t){
        return (long long)t.tv_sec * 1000L + (long long)(t.tv_nsec / MICRO_SEC);
    }
};

class MonotonicClock : public Clock {
public:
    virtual ~MonotonicClock(){};

    long long getTime() override {
        struct timespec t;
        clock_gettime(CLOCK_MONOTONIC, &t);
        return convertTime(t);
    }

};

class WallClock : public Clock {
public:
    virtual ~WallClock(){};

    long long getTime() override {
        struct timespec t;
        clock_gettime(CLOCK_REALTIME, &t);
        return convertTime(t);
    }
};

long long TizenDeviceImpl::getSystemTime(TimeRequestType type)
{
    std::unique_ptr<Clock> clock;
    if (type == TimeRequestType::MONOTONIC)
        clock = std::make_unique<MonotonicClock>();
    else if (type == TimeRequestType::WALLCLOCK)
        clock = std::make_unique<WallClock>();

    return clock->getTime();

}

const Size2D<int> TizenDeviceImpl::getScreenSize()
{
    TizenDeviceImpl *obj = static_cast<TizenDeviceImpl *>(this);
    LOGI("getScreenSize : %d %d", obj->mScreenSize.width , obj->mScreenSize.height);
    return obj->mScreenSize;
}

int TizenDeviceImpl::grabTouchSeqNumber()
{
    for (unsigned int i = 0 ; i < MAX_FINGER_NUMBER; i++) {
        if (mTouchSeq.count(i) == 0) {
            mTouchSeq.insert(i);
            return i;
        }
    }
    return -1;
}

bool TizenDeviceImpl::releaseTouchSeqNumber(int seq)
{
    auto k = mTouchSeq.find(seq);
    if (k != mTouchSeq.end()) {
        mTouchSeq.erase(k);
        return true;
    }
    return false;
}
