#include "DeviceImpl/TizenImpl.h"
#include <loguru.hpp>

#include <functional>
#include <tuple>
#include <iostream>
#include <memory>

#include <stdlib.h>
#include <time.h>
#include <Ecore.h>

TizenImpl::TizenImpl()
: mFakeTouchHandle{0}, mFakeKeyboardHandle{0}, mFakeWheelHandle{0}, isTimerStarted{false}, mTouchSeq{}
{
    LOG_SCOPE_F(INFO, "device implementation init");
    ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
        TizenImpl *obj = static_cast<TizenImpl*>(data);
        obj->mFakeTouchHandle = efl_util_input_initialize_generator(EFL_UTIL_INPUT_DEVTYPE_TOUCHSCREEN);
        obj->mFakeKeyboardHandle =
            efl_util_input_initialize_generator(EFL_UTIL_INPUT_DEVTYPE_KEYBOARD);
        obj->mFakeWheelHandle = efl_util_input_initialize_generator(EFL_UTIL_INPUT_DEVTYPE_POINTER);
        return NULL;
    }, this);
}

TizenImpl::~TizenImpl()
{
    ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
        TizenImpl *obj = static_cast<TizenImpl*>(data);
        efl_util_input_deinitialize_generator(obj->mFakeTouchHandle);
        efl_util_input_deinitialize_generator(obj->mFakeKeyboardHandle);
        efl_util_input_deinitialize_generator(obj->mFakeWheelHandle);
        return NULL;
    }, this);
}

bool TizenImpl::click(const int x, const int y)
{
    return click(x, y, INTV_CLICK);
}

bool TizenImpl::click(const int x, const int y, const unsigned int intv)
{
    int seq = touchDown(x, y);
    if (seq < 0) return false;
    usleep(intv * MSEC_PER_SEC);
    touchUp(x, y, seq);

    return true;
}


int TizenImpl::touchDown(const int x, const int y)
{
    int seq = grabTouchSeqNumber();
    LOG_F(INFO, "touch down %d %d , seq:%d", x, y, seq);
    if (seq >= 0) {
        auto args = std::make_tuple(this, x, y, seq);
        long result = (long)ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
            TizenImpl *obj;
            int x, y, seq;
            std::tie(obj, x, y, seq) = *static_cast<std::tuple<TizenImpl*, int, int, int>*>(data);
            return (void*)efl_util_input_generate_touch(obj->mFakeTouchHandle, seq, EFL_UTIL_INPUT_TOUCH_BEGIN,
                                    x, y);

        }, (void*)(&args));

        if (result != EFL_UTIL_ERROR_NONE) {
            releaseTouchSeqNumber(seq);
            return -1;
        }
    }
    return seq;
}

bool TizenImpl::touchMove(const int x, const int y, const int seq)
{
    LOG_F(INFO, "touch move %d %d, seq:%d", x, y, seq);
    if (seq >= 0) {
        auto args = std::make_tuple(this, x, y, seq);
        long result = (long)ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
            TizenImpl *obj;
            int x, y, seq;
            std::tie(obj, x, y, seq) = *static_cast<std::tuple<TizenImpl*, int, int, int>*>(data);

            return (void*)efl_util_input_generate_touch(obj->mFakeTouchHandle, seq, EFL_UTIL_INPUT_TOUCH_UPDATE,
                                    x, y);

        }, (void*)(&args));
        return result == EFL_UTIL_ERROR_NONE;
    }
    return false;
}

bool TizenImpl::touchUp(const int x, const int y, const int seq)
{
    LOG_F(INFO, "touch up %d %d, seq:%d", x, y, seq);
    if (seq >= 0) {
        auto args = std::make_tuple(this, x, y, seq);
        long result = (long)ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
            TizenImpl *obj;
            int x, y, seq;
            std::tie(obj, x, y, seq) = *static_cast<std::tuple<TizenImpl*, int, int, int>*>(data);
            return (void*)efl_util_input_generate_touch(obj->mFakeTouchHandle, seq, EFL_UTIL_INPUT_TOUCH_END,
                                    x, y);
        }, (void*)(&args));
        return releaseTouchSeqNumber(seq) && result == EFL_UTIL_ERROR_NONE;
    }
    return false;
}

bool TizenImpl::wheelUp(int amount, const int durationMs)
{
    LOG_F(INFO, "wheel up %d for %d", amount, durationMs);
    auto args = std::make_tuple(this);
    long result = -1;
    for (int i = 0; i < amount; i++){
        result = (long)ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
                TizenImpl *obj;
                std::tie(obj) = *static_cast<std::tuple<TizenImpl*>*>(data);
            return (void*)efl_util_input_generate_wheel(obj->mFakeWheelHandle, EFL_UTIL_INPUT_POINTER_WHEEL_HORZ, 1);
        }, (void*)(&args));
        usleep(durationMs*MSEC_PER_SEC/amount);
    }

    return result == EFL_UTIL_ERROR_NONE;
}

bool TizenImpl::wheelDown(int amount, const int durationMs)
{
    LOG_F(INFO, "wheel down %d for %d", amount, durationMs);
    auto args = std::make_tuple(this);
    long result = -1;
    for (int i = 0; i < amount; i++){
        result = (long)ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
                TizenImpl *obj;
                std::tie(obj) = *static_cast<std::tuple<TizenImpl*>*>(data);
                return (void*)efl_util_input_generate_wheel(obj->mFakeWheelHandle, EFL_UTIL_INPUT_POINTER_WHEEL_HORZ, -1);
            return NULL;
        }, (void*)(&args));
        usleep(durationMs*MSEC_PER_SEC/amount);
    }

    return result == EFL_UTIL_ERROR_NONE;
}
void TizenImpl::startTimer(void)
{
    isTimerStarted = true;
    clock_gettime(CLOCK_MONOTONIC, &tStart);
}

int TizenImpl::stopTimer(void)
{
    struct timespec tEnd;
    if (!isTimerStarted) return 0;
    isTimerStarted = false;
    clock_gettime(CLOCK_MONOTONIC, &tEnd);
    return ((tEnd.tv_sec + tEnd.tv_nsec/1000000000.0) - (tStart.tv_sec + tStart.tv_nsec/1000000000.0)) * 1000000;
}

bool TizenImpl::drag(const int sx, const int sy, const int ex, const int ey,
                         const int steps, const int durationMs)
{
    int i, j;
    int _durationUs, _steps, _stepUs;
    int dur;
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
    LOG_SCOPE_F(INFO, "flicking (%d, %d) -> (%d, %d) for (%d ms)", sx, sy, ex, ey, durationMs);
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

bool TizenImpl::pressBack(KeyRequestType type)
{
    return pressKeyCode("XF86Back", type);
}

bool TizenImpl::pressHome(KeyRequestType type)
{
    return pressKeyCode("XF86Home", type);
}

bool TizenImpl::pressMenu(KeyRequestType type)
{
    return pressKeyCode("XF86Menu", type);
}

bool TizenImpl::pressVolUp(KeyRequestType type)
{
    return pressKeyCode("XF86AudioRaiseVolume", type);
}

bool TizenImpl::pressVolDown(KeyRequestType type)
{
    return pressKeyCode("XF86AudioLowerVolume", type);
}

bool TizenImpl::pressPower(KeyRequestType type)
{
    return pressKeyCode("XF86PowerOff", type);
}

bool TizenImpl::pressKeyCode(std::string keycode, KeyRequestType type)
{
    if (type == KeyRequestType::STROKE)
        return strokeKeyCode(keycode, INTV_SHORTSTROKE);
    else if (type == KeyRequestType::LONG_STROKE)
        return strokeKeyCode(keycode, INTV_LONGSTROKE);
    else if (type == KeyRequestType::PRESS)
        return pressKeyCode(keycode);
    else if (type == KeyRequestType::RELEASE)
        return releaseKeyCode(keycode);
    return false;
}

bool TizenImpl::strokeKeyCode(std::string keycode, unsigned int intv)
{
    pressKeyCode(keycode);
    usleep(intv * 1000);
    releaseKeyCode(keycode);
    return true;
}

bool TizenImpl::pressKeyCode(std::string keycode)
{
    auto args = std::make_tuple(this, keycode);
    long result = (long)ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
        TizenImpl *obj;
        std::string keycode;
        std::tie(obj, keycode) = *static_cast<std::tuple<TizenImpl*, std::string>*>(data);
        return (void*)efl_util_input_generate_key(obj->mFakeKeyboardHandle, keycode.c_str(), 1);
    }, (void*)(&args));
    ecore_main_loop_thread_safe_call_sync([](void *data)->void*{return NULL;}, NULL);
    return result == EFL_UTIL_ERROR_NONE;
}

bool TizenImpl::releaseKeyCode(std::string keycode)
{
    auto args = std::make_tuple(this, keycode);
    long result = (long)ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
        TizenImpl *obj;
        std::string keycode;
        std::tie(obj, keycode) = *static_cast<std::tuple<TizenImpl*, std::string>*>(data);
        return (void*)efl_util_input_generate_key(obj->mFakeKeyboardHandle, keycode.c_str(), 0);
    }, (void*)(&args));
    ecore_main_loop_thread_safe_call_sync([](void *data)->void*{return NULL;}, NULL);
    return result == EFL_UTIL_ERROR_NONE;
}

bool TizenImpl::takeScreenshot(std::string path, float scale, int quality)
{
    return false;
}

class Clock {
public:
    virtual ~Clock(){};

public:
    virtual long long getTime() = 0;
protected:
    long long convertTime(struct timespec t){
        return (long long)t.tv_sec * 1000L + (long long)(t.tv_nsec / 1000000);
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

long long TizenImpl::getSystemTime(TimeRequestType type)
{
    std::unique_ptr<Clock> clock;
    if (type == TimeRequestType::MONOTONIC)
        clock = std::make_unique<MonotonicClock>();
    else if (type == TimeRequestType::WALLCLOCK)
        clock = std::make_unique<WallClock>();

    return clock->getTime();

}

int TizenImpl::grabTouchSeqNumber()
{
    for (int i = 0 ; i < MAX_FINGER_NUMBER; i++) {
        if (mTouchSeq.count(i) == 0) {
            mTouchSeq.insert(i);
            return i;
        }
    }
    return -1;
}

bool TizenImpl::releaseTouchSeqNumber(int seq)
{
    auto k = mTouchSeq.find(seq);
    if (k != mTouchSeq.end()) {
        mTouchSeq.erase(k);
        return true;
    }
    return false;
}