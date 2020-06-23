#ifndef DEVICE_GENERAL_H
#define DEVICE_GENERAL_H
#include "config.h"

#include "IDevice.h"
#include <set>

#ifdef GBS_BUILD
#include <efl_util.h>
#endif
/**
 * @brief TizenImpl Class
 * @since_tizen 5.5
 */
class TizenImpl : public IDevice {
public:
    TizenImpl();
    ~TizenImpl();

    bool click(const int x, const int y) override;
    bool click(const int x, const int y, const unsigned int intv) override;

    bool drag(const int sx, const int sy, const int ex, const int ey,
              const int steps, const int durationMs) override;

    int touchDown(const int x, const int y) override;
    bool touchMove(const int x, const int y, const int seq) override;
    bool touchUp(const int x, const int y, const int seq) override;

    bool wheelUp(int amount, const int durationMs) override;
    bool wheelDown(int amount, const int durationMs) override;

    bool pressBack(KeyRequestType type) override;
    bool pressHome(KeyRequestType type) override;
    bool pressMenu(KeyRequestType type) override;
    bool pressVolUp(KeyRequestType type) override;
    bool pressVolDown(KeyRequestType type) override;
    bool pressPower(KeyRequestType type) override;
    bool pressKeyCode(std::string keycode, KeyRequestType type) override;
    bool takeScreenshot(std::string path, float scale, int quality) override;
    long long getSystemTime(TimeRequestType type) override;

protected:
    bool strokeKeyCode(std::string keycode, unsigned int intv);
    bool pressKeyCode(std::string keycode);
    bool releaseKeyCode(std::string keycode);

    int grabTouchSeqNumber();
    bool releaseTouchSeqNumber(int seq);

private:
    void startTimer(void);
    int stopTimer(void);

private:
    efl_util_inputgen_h mFakeTouchHandle;
    efl_util_inputgen_h mFakeKeyboardHandle;
    efl_util_inputgen_h mFakeWheelHandle;
    static const unsigned int INTV_CLICK = 5;
    static const unsigned int INTV_SHORTSTROKE = 100;
    static const unsigned int INTV_LONGSTROKE = 2000;

    static const unsigned int INTV_MINIMUM_DRAG_MS = 25;
    static const unsigned int INTV_MINIMUM_USLEEP = 1000;
    static const unsigned int MINIMUM_DURATION_DRAG = 100;
    static const unsigned int MSEC_PER_SEC = 1000;
    static const unsigned int MAX_FINGER_NUMBER = 2;

    struct timespec tStart;
    bool isTimerStarted;

    std::set<int> mTouchSeq;
};

#endif