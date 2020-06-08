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
              const int steps, const int durationMs) override;

    bool touchDown(const int x, const int y) override;
    bool touchMove(const int x, const int y) override;
    bool touchUp(const int x, const int y) override;

    bool wheelUp(int amount, const int durationMs) override;
    bool wheelDown(int amount, const int durationMs) override;

    bool pressBack() override;
    bool pressHome() override;
    bool pressMenu() override;
    bool pressVolUp() override;
    bool pressVolDown() override;
    bool pressPower() override;
    bool pressKeyCode(std::string keycode) override;
    bool takeScreenshot(std::string path, float scale, int quality) override;
    long long getSystemTime(TypeRequestType type) override;

protected:
    bool pressKeyCode(std::string keycode, unsigned int intv);

private:
    void startTimer(void);
    int stopTimer(void);

private:
    efl_util_inputgen_h mFakeTouchHandle;
    efl_util_inputgen_h mFakeKeyboardHandle;
    efl_util_inputgen_h mFakeWheelHandle;
    static const unsigned int INTV_CLICK = 5;
    static const unsigned int INTV_KEYPRESS = 10;
    static const unsigned int INTV_MINIMUM_DRAG_MS = 25;
    static const unsigned int INTV_MINIMUM_USLEEP = 1000;
    static const unsigned int MINIMUM_DURATION_DRAG = 100;
    static const unsigned int MSEC_PER_SEC = 1000;

    struct timespec t0;
    bool isTimerStarted;
};

#endif