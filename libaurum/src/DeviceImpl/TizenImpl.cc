#include "DeviceImpl/TizenImpl.h"
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

TizenImpl::TizenImpl()
{
    LOG_SCOPE_F(INFO, "device implementation init");
    ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
        TizenImpl *obj = static_cast<TizenImpl*>(data);
        obj->mFakeTouchHandle = efl_util_input_initialize_generator(EFL_UTIL_INPUT_DEVTYPE_TOUCHSCREEN);
        obj->mFakeKeyboardHandle =
            efl_util_input_initialize_generator(EFL_UTIL_INPUT_DEVTYPE_KEYBOARD);
        return NULL;
    }, this);
}

TizenImpl::~TizenImpl()
{
    ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
        TizenImpl *obj = static_cast<TizenImpl*>(data);
        efl_util_input_deinitialize_generator(obj->mFakeTouchHandle);
        efl_util_input_deinitialize_generator(obj->mFakeKeyboardHandle);
        return NULL;
    }, this);
}

bool TizenImpl::click(const int x, const int y)
{
    return click(x, y, INTV_CLICK);
}

bool TizenImpl::click(const int x, const int y, const unsigned int intv)
{
    LOG_SCOPE_F(INFO, "click at (%d, %d)", x, y);
    auto args = std::make_tuple(this, x, y, intv);
    ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
        TizenImpl *obj;
        int x, y;
        unsigned int intv;
        std::tie(obj, x, y, intv) = *static_cast<std::tuple<TizenImpl*, int, int, unsigned int>*>(data);

        efl_util_input_generate_touch(obj->mFakeTouchHandle, 0,
                                    EFL_UTIL_INPUT_TOUCH_BEGIN, x, y);
        usleep(intv*10000);
        efl_util_input_generate_touch(obj->mFakeTouchHandle, 0, EFL_UTIL_INPUT_TOUCH_END,
                                    x, y);
        return NULL;
    }, (void*)(&args));
    return true;
}


bool TizenImpl::touchDown(const int x, const int y)
{
    LOG_F(INFO, "%d %d", x, y);
    auto args = std::make_tuple(this, x, y);
    ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
        TizenImpl *obj;
        int x, y;
        std::tie(obj, x, y) = *static_cast<std::tuple<TizenImpl*, int, int>*>(data);
        efl_util_input_generate_touch(obj->mFakeTouchHandle, 0, EFL_UTIL_INPUT_TOUCH_BEGIN,
                                  x, y);

        return NULL;
    }, (void*)(&args));
    return true;
}

bool TizenImpl::touchMove(const int x, const int y)
{
    LOG_F(INFO, "%d %d", x, y);
    auto args = std::make_tuple(this, x, y);
    ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
        TizenImpl *obj;
        int x, y;
        std::tie(obj, x, y) = *static_cast<std::tuple<TizenImpl*, int, int>*>(data);

        efl_util_input_generate_touch(obj->mFakeTouchHandle, 0, EFL_UTIL_INPUT_TOUCH_UPDATE,
                                  x, y);

        return NULL;
    }, (void*)(&args));
    return true;
}

bool TizenImpl::touchUp(const int x, const int y)
{
    LOG_F(INFO, "touch up %d %d", x, y);
    auto args = std::make_tuple(this, x, y);
    ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
        TizenImpl *obj;
        int x, y;
        std::tie(obj, x, y) = *static_cast<std::tuple<TizenImpl*, int, int>*>(data);
        efl_util_input_generate_touch(obj->mFakeTouchHandle, 0, EFL_UTIL_INPUT_TOUCH_END,
                                  x, y);

        return NULL;
    }, (void*)(&args));

    return true;
}

bool TizenImpl::drag(const int sx, const int sy, const int ex, const int ey,
                         const int duration)
{
    int i, j, dirX, dirY, stepX, stepY;
    int dur;

    // TODO fixed fps implementation
    if (duration < 10) dur = 10;
    else dur = duration;

    dirX = sx > ex ? -1 : 1;
    dirY = sy > ey ? -1 : 1;

    stepX = (ex - sx)/20;
    stepY = (ey - sy)/20;

    LOG_SCOPE_F(INFO, "flicking (%d, %d) -> (%d, %d) for (%d ms)", sx, sy, ex, ey, duration);

    auto args1 = std::make_tuple(this, sx, sy);
    ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
        TizenImpl *obj;
        int i, j;
        std::tie(obj, i, j) = *static_cast<std::tuple<TizenImpl*, int, int>*>(data);
        efl_util_input_generate_touch(obj->mFakeTouchHandle, 0,
                                        EFL_UTIL_INPUT_TOUCH_BEGIN, i, j);
        LOG_F(INFO, "flick begin (%d, %d)", i, j);
        return NULL;
    }, (void*)(&args1));

    i = sx, j = sy;
    do {
        std::tuple<TizenImpl*, int, int> args;
        args = std::make_tuple(this, i, j);
        ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
            TizenImpl *obj;
            int i, j;
            std::tie(obj, i, j) = *static_cast<std::tuple<TizenImpl*, int, int>*>(data);
            LOG_F(INFO, "flick move (%d, %d)", i, j);

            efl_util_input_generate_touch(obj->mFakeTouchHandle, 0,
                                            EFL_UTIL_INPUT_TOUCH_UPDATE, i, j);
            return NULL;
        }, (void*)(&args));

        usleep(dur * 1000);
        LOG_F(INFO, "sleep ms %d", dur);
        i += stepX;
        j += stepY;
    } while(i*dirX <= ex && j*dirY <= ey);

    auto args2 = std::make_tuple(this, ex, ey);
    ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
        TizenImpl *obj;
        int i, j;
        std::tie(obj, i, j) = *static_cast<std::tuple<TizenImpl*, int, int>*>(data);
        efl_util_input_generate_touch(obj->mFakeTouchHandle, 0, EFL_UTIL_INPUT_TOUCH_END,
                                        i, j);
        LOG_SCOPE_F(INFO, "flick end: (%d, %d)", i, j);
        return NULL;
    }, (void*)(&args2));

    ecore_main_loop_thread_safe_call_sync([](void *data)->void*{return NULL;}, NULL);

    return true;
}

bool TizenImpl::pressBack()
{
    return pressKeyCode("XF86Back");
}

bool TizenImpl::pressHome()
{
    return pressKeyCode("XF86Home");
}

bool TizenImpl::pressMenu()
{
    return pressKeyCode("XF86Menu");
}

bool TizenImpl::pressVolUp()
{
    return pressKeyCode("XF86AudioRaiseVolume");
}

bool TizenImpl::pressVolDown()
{
    return pressKeyCode("XF86AudioLowerVolume");
}

bool TizenImpl::pressPower()
{
    return pressKeyCode("XF86PowerOff");
}

bool TizenImpl::pressKeyCode(std::string keycode)
{
    return pressKeyCode(keycode, INTV_KEYPRESS);
}

bool TizenImpl::pressKeyCode(std::string keycode, unsigned int intv)
{
    auto args = std::make_tuple(this, keycode);
    ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
        TizenImpl *obj;
        std::string keycode;
        std::tie(obj, keycode) = *static_cast<std::tuple<TizenImpl*, std::string>*>(data);
        efl_util_input_generate_key(obj->mFakeKeyboardHandle, keycode.c_str(), 1);
        return NULL;
    }, (void*)(&args));

    ecore_main_loop_thread_safe_call_sync([](void *data)->void*{return NULL;}, NULL);
    usleep(intv * 1000);

    ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
        TizenImpl *obj;
        std::string keycode;
        std::tie(obj, keycode) = *static_cast<std::tuple<TizenImpl*, std::string>*>(data);
        efl_util_input_generate_key(obj->mFakeKeyboardHandle, keycode.c_str(), 0);
        return NULL;
    }, (void*)(&args));

    return true;
}

bool TizenImpl::takeScreenshot(std::string path, float scale, int quality)
{
    efl_util_screenshot_h screenshot = NULL;
    tbm_surface_h tbm_surface = NULL;

    int width, height;
    system_info_get_platform_int("http://tizen.org/feature/screen.width", &width);
    system_info_get_platform_int("http://tizen.org/feature/screen.height", &height);

    screenshot = efl_util_screenshot_initialize(width, height);

    if (screenshot) {
        tbm_surface = efl_util_screenshot_take_tbm_surface(screenshot);
        if (tbm_surface) {
            char path_cstr[PATH_MAX];
            strcpy(path_cstr, path.c_str());
            tdm_helper_dump_buffer(tbm_surface, path_cstr);
            sync();
        }
        efl_util_screenshot_deinitialize(screenshot);
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

long long TizenImpl::getSystemTime(TypeRequestType type)
{
    std::unique_ptr<Clock> clock;
    if (type == TypeRequestType::MONOTONIC)
        clock = std::make_unique<MonotonicClock>();
    else if (type == TypeRequestType::WALLCLOCK)
        clock = std::make_unique<WallClock>();

    return clock->getTime();

}
