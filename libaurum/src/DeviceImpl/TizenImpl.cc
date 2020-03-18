#include "DeviceImpl/TizenImpl.h"

#include <stdlib.h>
#include <iostream>

#include <stdlib.h>
#include "loguru.hpp"

TizenImpl::TizenImpl()
{
    LOG_SCOPE_F(INFO, "device implementation init");
#ifdef GBSBUILD
    mFakeTouchHandle =
        efl_util_input_initialize_generator(EFL_UTIL_INPUT_DEVTYPE_TOUCHSCREEN);
    mFakeKeyboardHandle =
        efl_util_input_initialize_generator(EFL_UTIL_INPUT_DEVTYPE_KEYBOARD);
#endif
}

TizenImpl::~TizenImpl()
{
#ifdef GBSBUILD
    efl_util_input_deinitialize_generator(mFakeTouchHandle);
    efl_util_input_deinitialize_generator(mFakeKeyboardHandle);
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
    efl_util_input_generate_touch(mFakeTouchHandle, 0,
                                  EFL_UTIL_INPUT_TOUCH_BEGIN, x, y);
    usleep(intv*10000);
    efl_util_input_generate_touch(mFakeTouchHandle, 0, EFL_UTIL_INPUT_TOUCH_END,
                                  x, y);
    return true;
#else
    return false;
#endif
}


bool TizenImpl::touchDown(const int x, const int y)
{
#ifdef GBSBUILD
    LOG_F(INFO, "%d %d", x, y);
    efl_util_input_generate_touch(mFakeTouchHandle, 0,
                                  EFL_UTIL_INPUT_TOUCH_BEGIN, x, y);
#endif
    return true;
}

bool TizenImpl::touchMove(const int x, const int y)
{
#ifdef GBSBUILD
    LOG_F(INFO, "%d %d", x, y);
    efl_util_input_generate_touch(mFakeTouchHandle, 0,
                                  EFL_UTIL_INPUT_TOUCH_UPDATE, x, y);
#endif
    return true;
}

bool TizenImpl::touchUp(const int x, const int y)
{
#ifdef GBSBUILD
    LOG_F(INFO, "%d %d", x, y);
    efl_util_input_generate_touch(mFakeTouchHandle, 0, EFL_UTIL_INPUT_TOUCH_END,
                                  x, y);
#endif
    return true;
}

bool TizenImpl::drag(const int sx, const int sy, const int ex, const int ey,
                         const int duration)
{
#ifdef GBSBUILD
    int i, j;
    int dur;
    // TODO fixed fps implementation
    if (duration < 10) dur = 10;
    else dur = duration;

    i = sx, j = sy;
    LOG_SCOPE_F(INFO, "flicking (%d, %d)", i, j);

    efl_util_input_generate_touch(mFakeTouchHandle, 0,
                                  EFL_UTIL_INPUT_TOUCH_BEGIN, i, j);
    for (; i <= ex && j <= ey; i += (ex - sx) / 10, j += (ey - sy) / 10) {
        efl_util_input_generate_touch(mFakeTouchHandle, 0,
                                      EFL_UTIL_INPUT_TOUCH_UPDATE, i, j);
        usleep(dur * 1000);
        LOG_SCOPE_F(INFO, "flicking (%d, %d)", i, j);
    }
    LOG_SCOPE_F(INFO, "flicking (%d, %d)", i, j);
    efl_util_input_generate_touch(mFakeTouchHandle, 0, EFL_UTIL_INPUT_TOUCH_END,
                                  i, j);
#endif
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
#ifdef GBSBUILD
    efl_util_input_generate_key(mFakeKeyboardHandle, keycode.c_str(), 1);
    efl_util_input_generate_key(mFakeKeyboardHandle, keycode.c_str(), 0);
#endif
    return true;
}

bool TizenImpl::takeScreenshot(std::string path, float scale, int quality)
{
    return false;
}
