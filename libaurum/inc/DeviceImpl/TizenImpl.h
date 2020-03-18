#ifndef DEVICE_GENERAL_H
#define DEVICE_GENERAL_H
#include "config.h"

#include "IDevice.h"

#ifdef GBS_BUILD
#include <efl_util.h>
#endif

class TizenImpl : public IDevice {
public:
    TizenImpl();
    ~TizenImpl();

    bool click(const int x, const int y) override;
    bool click(const int x, const int y, const unsigned int intv) override;

    bool drag(const int sx, const int sy, const int ex, const int ey,
              const int steps) override;

    bool touchDown(const int x, const int y) override;
    bool touchMove(const int x, const int y) override;
    bool touchUp(const int x, const int y) override;

    bool pressBack() override;
    bool pressHome() override;
    bool pressMenu() override;
    bool pressVolUp() override;
    bool pressVolDown() override;
    bool pressPower() override;
    bool pressKeyCode(std::string keycode) override;
    bool takeScreenshot(std::string path, float scale, int quality) override;

private:
#ifdef GBS_BUILD
    efl_util_inputgen_h mFakeTouchHandle;
    efl_util_inputgen_h mFakeKeyboardHandle;
#endif
    static const unsigned int INTV_CLICK = 5;
};

#endif