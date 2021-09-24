#pragma once

#include "config.h"
#include "IDevice.h"

#include <set>
#include <efl_util.h>

class TizenDeviceImpl : public IDevice {
public:
    TizenDeviceImpl();

    ~TizenDeviceImpl();

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
    /**
     * @brief Press and release given key during intv.
     *
     * @since_tizen 5.5
     */
    bool strokeKeyCode(std::string keycode, unsigned int intv);

    /**
     * @brief Press given key.
     *
     * @since_tizen 5.5
     */
    bool pressKeyCode(std::string keycode);

    /**
     * @brief Release given key.
     *
     * @since_tizen 5.5
     */
    bool releaseKeyCode(std::string keycode);

    /**
     * @brief Increase touch count and return the number to manage touch count.
     *
     * @since_tizen 5.5
     */
    int grabTouchSeqNumber();

    /**
     * @brief Delete given touch number.
     *
     * @since_tizen 5.5
     */
    bool releaseTouchSeqNumber(int seq);

private:
    /**
     * @brief Timer utility.
     *
     * @since_tizen 5.5
     */
    void startTimer(void);

    /**
     * @brief Timer utility.
     *
     * @since_tizen 5.5
     */
    int stopTimer(void);

private:
    efl_util_inputgen_h mFakeTouchHandle;

    efl_util_inputgen_h mFakeKeyboardHandle;

    efl_util_inputgen_h mFakeWheelHandle;

    static const int INTV_CLICK = 5;

    static const int INTV_SHORTSTROKE = 100;

    static const int INTV_LONGSTROKE = 2000;

    static const int INTV_MINIMUM_DRAG_MS = 25;

    static const int INTV_MINIMUM_USLEEP = 1000;

    static const int MINIMUM_DURATION_DRAG = 100;

    static const unsigned int MSEC_PER_SEC = 1000;

    static const unsigned int MAX_FINGER_NUMBER = 2;

    struct timespec tStart;

    bool isTimerStarted;

    std::set<int> mTouchSeq;
};