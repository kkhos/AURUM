#include "TizenDeviceImpl.h"
#include <loguru.hpp>

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

TizenDeviceImpl::TizenDeviceImpl()
: mFakeTouchHandle{0}, mFakeKeyboardHandle{0}, mFakeWheelHandle{0}, tStart{}, isTimerStarted{false}, mTouchSeq{}
{
    loguru::add_file("/tmp/aurum.log", loguru::Append, loguru::Verbosity_MAX);
    loguru::g_stderr_verbosity = loguru::Verbosity_ERROR;

    LOG_SCOPE_F(INFO, "device implementation init");
    ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
        TizenDeviceImpl *obj = static_cast<TizenDeviceImpl*>(data);
        obj->mFakeTouchHandle = efl_util_input_initialize_generator(EFL_UTIL_INPUT_DEVTYPE_TOUCHSCREEN);
        obj->mFakeKeyboardHandle =
            efl_util_input_initialize_generator(EFL_UTIL_INPUT_DEVTYPE_KEYBOARD);
        obj->mFakeWheelHandle = efl_util_input_initialize_generator(EFL_UTIL_INPUT_DEVTYPE_POINTER);
        return NULL;
    }, this);
}

TizenDeviceImpl::~TizenDeviceImpl()
{
    ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
        TizenDeviceImpl *obj = static_cast<TizenDeviceImpl*>(data);
        efl_util_input_deinitialize_generator(obj->mFakeTouchHandle);
        efl_util_input_deinitialize_generator(obj->mFakeKeyboardHandle);
        efl_util_input_deinitialize_generator(obj->mFakeWheelHandle);
        return NULL;
    }, this);
}

bool TizenDeviceImpl::click(const int x, const int y)
{
    return click(x, y, INTV_CLICK);
}

bool TizenDeviceImpl::click(const int x, const int y, const unsigned int intv)
{
    LOG_SCOPE_F(INFO, "click %d %d , intv:%d", x, y, intv);

    int seq = touchDown(x, y);
    if (seq < 0) return false;
    usleep(intv * MSEC_PER_SEC);
    touchUp(x, y, seq);

    return true;
}


int TizenDeviceImpl::touchDown(const int x, const int y)
{
    int seq = grabTouchSeqNumber();
    LOG_SCOPE_F(INFO, "touch down %d %d , seq:%d", x, y, seq);
    if (seq >= 0) {
        auto args = std::make_tuple(this, x, y, seq);
        long result = (long)ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
            TizenDeviceImpl *obj;
            int x, y, seq;
            std::tie(obj, x, y, seq) = *static_cast<std::tuple<TizenDeviceImpl*, int, int, int>*>(data);
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

bool TizenDeviceImpl::touchMove(const int x, const int y, const int seq)
{
    LOG_SCOPE_F(INFO, "touch move %d %d, seq:%d", x, y, seq);
    if (seq >= 0) {
        auto args = std::make_tuple(this, x, y, seq);
        long result = (long)ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
            TizenDeviceImpl *obj;
            int x, y, seq;
            std::tie(obj, x, y, seq) = *static_cast<std::tuple<TizenDeviceImpl*, int, int, int>*>(data);

            return (void*)efl_util_input_generate_touch(obj->mFakeTouchHandle, seq, EFL_UTIL_INPUT_TOUCH_UPDATE,
                                    x, y);

        }, (void*)(&args));
        return result == EFL_UTIL_ERROR_NONE;
    }
    return false;
}

bool TizenDeviceImpl::touchUp(const int x, const int y, const int seq)
{
    LOG_SCOPE_F(INFO, "touch up %d %d, seq:%d", x, y, seq);
    if (seq >= 0) {
        auto args = std::make_tuple(this, x, y, seq);
        long result = (long)ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
            TizenDeviceImpl *obj;
            int x, y, seq;
            std::tie(obj, x, y, seq) = *static_cast<std::tuple<TizenDeviceImpl*, int, int, int>*>(data);
            return (void*)efl_util_input_generate_touch(obj->mFakeTouchHandle, seq, EFL_UTIL_INPUT_TOUCH_END,
                                    x, y);
        }, (void*)(&args));
        return releaseTouchSeqNumber(seq) && result == EFL_UTIL_ERROR_NONE;
    }
    return false;
}

bool TizenDeviceImpl::wheelUp(int amount, const int durationMs)
{
    LOG_SCOPE_F(INFO, "wheel up %d for %d", amount, durationMs);
    auto args = std::make_tuple(this);
    long result = -1;
    for (int i = 0; i < amount; i++){
        result = (long)ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
                TizenDeviceImpl *obj;
                std::tie(obj) = *static_cast<std::tuple<TizenDeviceImpl*>*>(data);
            return (void*)efl_util_input_generate_wheel(obj->mFakeWheelHandle, EFL_UTIL_INPUT_POINTER_WHEEL_HORZ, 1);
        }, (void*)(&args));
        usleep(durationMs*MSEC_PER_SEC/amount);
    }

    return result == EFL_UTIL_ERROR_NONE;
}

bool TizenDeviceImpl::wheelDown(int amount, const int durationMs)
{
    LOG_SCOPE_F(INFO, "wheel down %d for %d", amount, durationMs);
    auto args = std::make_tuple(this);
    long result = -1;
    for (int i = 0; i < amount; i++){
        result = (long)ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
                TizenDeviceImpl *obj;
                std::tie(obj) = *static_cast<std::tuple<TizenDeviceImpl*>*>(data);
                return (void*)efl_util_input_generate_wheel(obj->mFakeWheelHandle, EFL_UTIL_INPUT_POINTER_WHEEL_HORZ, -1);
            return NULL;
        }, (void*)(&args));
        usleep(durationMs*MSEC_PER_SEC/amount);
    }

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
    return ((tEnd.tv_sec + tEnd.tv_nsec/1000000000.0) - (tStart.tv_sec + tStart.tv_nsec/1000000000.0)) * 1000000;
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
    return false;
}

bool TizenDeviceImpl::strokeKeyCode(std::string keycode, unsigned int intv)
{
    pressKeyCode(keycode);
    usleep(intv * 1000);
    releaseKeyCode(keycode);
    return true;
}

bool TizenDeviceImpl::pressKeyCode(std::string keycode)
{
    auto args = std::make_tuple(this, keycode);
    long result = (long)ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
        TizenDeviceImpl *obj;
        std::string keycode;
        std::tie(obj, keycode) = *static_cast<std::tuple<TizenDeviceImpl*, std::string>*>(data);
        return (void*)efl_util_input_generate_key(obj->mFakeKeyboardHandle, keycode.c_str(), 1);
    }, (void*)(&args));
    ecore_main_loop_thread_safe_call_sync([](void *data)->void*{return NULL;}, NULL);
    return result == EFL_UTIL_ERROR_NONE;
}

bool TizenDeviceImpl::releaseKeyCode(std::string keycode)
{
    auto args = std::make_tuple(this, keycode);
    long result = (long)ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
        TizenDeviceImpl *obj;
        std::string keycode;
        std::tie(obj, keycode) = *static_cast<std::tuple<TizenDeviceImpl*, std::string>*>(data);
        return (void*)efl_util_input_generate_key(obj->mFakeKeyboardHandle, keycode.c_str(), 0);
    }, (void*)(&args));
    ecore_main_loop_thread_safe_call_sync([](void *data)->void*{return NULL;}, NULL);
    return result == EFL_UTIL_ERROR_NONE;
}

bool TizenDeviceImpl::takeScreenshot(std::string path, float scale, int quality)
{
    efl_util_screenshot_h screenshot = NULL;
    tbm_surface_h tbm_surface = NULL;

    int width = 0, height = 0;
    if (system_info_get_platform_int("http://tizen.org/feature/screen.width", &width) ||
        system_info_get_platform_int("http://tizen.org/feature/screen.height", &height))
        return false;

    screenshot = efl_util_screenshot_initialize(width, height);

    if (screenshot) {
        tbm_surface = efl_util_screenshot_take_tbm_surface(screenshot);
        if (tbm_surface) {
            tdm_helper_dump_buffer(tbm_surface, path.c_str());
            sync();
        } else {
            efl_util_screenshot_deinitialize(screenshot);
            return false;
        }
        efl_util_screenshot_deinitialize(screenshot);
    } else {
        return false;
    }

    return true;
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

long long TizenDeviceImpl::getSystemTime(TimeRequestType type)
{
    std::unique_ptr<Clock> clock;
    if (type == TimeRequestType::MONOTONIC)
        clock = std::make_unique<MonotonicClock>();
    else if (type == TimeRequestType::WALLCLOCK)
        clock = std::make_unique<WallClock>();

    return clock->getTime();

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
