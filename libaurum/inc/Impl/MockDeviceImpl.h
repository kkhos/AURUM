#pragma once

#include "config.h"
#include "IDevice.h"

#include <set>
#include <list>
#include <tuple>
#include <vector>
#include <map>

enum class MockKeyType {
    BACK,
    HOME,
    MENU,
    VOLUP,
    VOLDOWN,
    POWER,
    KEY
};

struct TouchData { int x; int y; long long stamp1; long long stamp2;};
class MockDeviceImpl : public IDevice {
public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    MockDeviceImpl();

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    ~MockDeviceImpl();

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool click(const int x, const int y) override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool click(const int x, const int y, const unsigned int intv) override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool drag(const int sx, const int sy, const int ex, const int ey,
              const int steps, const int durationMs) override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    int touchDown(const int x, const int y) override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool touchMove(const int x, const int y, const int seq) override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool touchUp(const int x, const int y, const int seq) override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool wheelUp(int amount, const int durationMs) override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool wheelDown(int amount, const int durationMs) override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool pressBack(KeyRequestType type) override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool pressHome(KeyRequestType type) override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool pressMenu(KeyRequestType type) override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool pressVolUp(KeyRequestType type) override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool pressVolDown(KeyRequestType type) override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool pressPower(KeyRequestType type) override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool pressKeyCode(std::string keycode, KeyRequestType type) override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool takeScreenshot(std::string path, float scale, int quality) override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    long long getSystemTime(TimeRequestType type) override;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    const Rect<int> getScreenSize() override;

protected:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool strokeKeyCode(std::string keycode, unsigned int intv);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool pressKeyCode(std::string keycode);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool releaseKeyCode(std::string keycode);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    int grabTouchSeqNumber();

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool releaseTouchSeqNumber(int seq);

private:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    void startTimer(void);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    int stopTimer(void);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    long long timeStamp(void);

public:
    /**
     * @brief TBD
     */
    static const int INTV_CLICK = 5;

    /**
     * @brief TBD
     */
    static const int INTV_SHORTSTROKE = 100;

    /**
     * @brief TBD
     */
    static const int INTV_LONGSTROKE = 2000;

    /**
     * @brief TBD
     */
    static const int INTV_MINIMUM_DRAG_MS = 25;

    /**
     * @brief TBD
     */
    static const int INTV_MINIMUM_USLEEP = 1000;

    /**
     * @brief TBD
     */
    static const int MINIMUM_DURATION_DRAG = 100;

    /**
     * @brief TBD
     */
    static const unsigned int MSEC_PER_SEC = 1000;

    /**
     * @brief TBD
     */
    static const unsigned int MAX_FINGER_NUMBER = 2;

    /**
     * @brief TBD
     */
    struct timespec tStart;

    /**
     * @brief TBD
     */
    bool isTimerStarted;

    /**
     * @brief TBD
     */
    std::set<int> mTouchSeq;

public:
    /**
     * @brief TBD
     */
    std::map<int, TouchData> mTouchRelease;

    /**
     * @brief TBD
     */
    std::map<int, TouchData> mTouchDown;

    /**
     * @brief TBD
     */
    std::vector<std::tuple<MockKeyType, KeyRequestType, std::string>> mKeyDevice;

    /**
     * @brief TBD
     */
    int mWheelDevice;

    /**
     * @brief TBD
     */
    Rect<int> mScreenSize;
};