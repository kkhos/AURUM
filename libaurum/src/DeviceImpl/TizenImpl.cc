#include "DeviceImpl/TizenImpl.h"

#include <functional>
#include <tuple>

#include <stdlib.h>
#include <iostream>
#include <Ecore.h>
#include "loguru.hpp"

TizenImpl::TizenImpl()
{
    LOG_SCOPE_F(INFO, "device implementation init");
#ifdef GBSBUILD
    ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
        TizenImpl *obj = static_cast<TizenImpl*>(data);
        obj->mFakeTouchHandle = efl_util_input_initialize_generator(EFL_UTIL_INPUT_DEVTYPE_TOUCHSCREEN);
        obj->mFakeKeyboardHandle =
            efl_util_input_initialize_generator(EFL_UTIL_INPUT_DEVTYPE_KEYBOARD);
    }, this);
#endif

}

TizenImpl::~TizenImpl()
{
#ifdef GBSBUILD
    ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
        TizenImpl *obj = static_cast<TizenImpl*>(data);
        efl_util_input_deinitialize_generator(obj->mFakeTouchHandle);
        efl_util_input_deinitialize_generator(obj->mFakeKeyboardHandle);
        return NULL;
    }, this);
#endif
}

bool TizenImpl::click(const int x, const int y)
{
    return click(x, y, INTV_CLICK);
}

bool TizenImpl::click(const int x, const int y, const unsigned int intv)
{
LOG_SCOPE_F(INFO, "click at (%d, %d)", x, y);
#ifdef GBSBUILD
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
#else
    return false;
#endif
}


bool TizenImpl::touchDown(const int x, const int y)
{
#ifdef GBSBUILD
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
#endif
    return true;
}

bool TizenImpl::touchMove(const int x, const int y)
{
#ifdef GBSBUILD
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
#endif
    return true;
}

bool TizenImpl::touchUp(const int x, const int y)
{
#ifdef GBSBUILD
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
#endif
    return false;
}

bool TizenImpl::drag(const int sx, const int sy, const int ex, const int ey,
                         const int duration)
{
#ifdef GBSBUILD
    auto args = std::make_tuple(this, sx, sy, ex, ey, duration);
    ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
        TizenImpl *obj;
        int sx, sy, ex, ey, duration;
        std::tie(obj, sx, sy, ex, ey, duration) = *static_cast<std::tuple<TizenImpl*, int, int, int, int, int>*>(data);

        int i, j;
        int dur;
        // TODO fixed fps implementation
        if (duration < 10) dur = 10;
        else dur = duration;

        i = sx, j = sy;
        LOG_SCOPE_F(INFO, "flicking (%d, %d)", i, j);

        efl_util_input_generate_touch(obj->mFakeTouchHandle, 0,
                                        EFL_UTIL_INPUT_TOUCH_BEGIN, i, j);
        for (; i <= ex && j <= ey; i += (ex - sx) / 10, j += (ey - sy) / 10) {
            efl_util_input_generate_touch(obj->mFakeTouchHandle, 0,
                                            EFL_UTIL_INPUT_TOUCH_UPDATE, i, j);
            usleep(dur * 1000);
            LOG_SCOPE_F(INFO, "flicking (%d, %d)", i, j);
        }
        LOG_SCOPE_F(INFO, "flicking (%d, %d)", i, j);
        efl_util_input_generate_touch(obj->mFakeTouchHandle, 0, EFL_UTIL_INPUT_TOUCH_END,
                                        i, j);

        return NULL;
    }, (void*)(&args));
    return true;
#endif
    return false;
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
#ifdef GBSBUILD
    auto args = std::make_tuple(this, keycode);
    ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
        TizenImpl *obj;
        std::string keycode;
        std::tie(obj, keycode) = *static_cast<std::tuple<TizenImpl*, std::string>*>(data);

        efl_util_input_generate_key(obj->mFakeKeyboardHandle, keycode.c_str(), 1);
        efl_util_input_generate_key(obj->mFakeKeyboardHandle, keycode.c_str(), 0);

        return NULL;
    }, (void*)(&args));
#endif
    return true;
}

bool TizenImpl::takeScreenshot(std::string path, float scale, int quality)
{
    return false;
}
