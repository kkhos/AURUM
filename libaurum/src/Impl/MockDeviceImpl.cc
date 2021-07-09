#include "MockDeviceImpl.h"

#include <functional>
#include <algorithm>
#include <tuple>
#include <iostream>
#include <memory>

#include <stdlib.h>
#include <time.h>

#include <stdio.h>
#include <unistd.h>

#include <chrono>
#include <thread>

#define NANO_SEC 1000000000.0
#define MICRO_SEC 1000000

MockDeviceImpl::MockDeviceImpl()
:tStart{}, isTimerStarted{false}, mTouchRelease{}, mTouchDown{}, mKeyDevice{}, mWheelDevice{0}
{
    mTouchRelease.emplace(1, TouchData{0,0,0,0});
    mTouchRelease.emplace(2, TouchData{0,0,0,0});
}

MockDeviceImpl::~MockDeviceImpl()
{
}

bool MockDeviceImpl::click(const int x, const int y)
{
    return click(x, y, INTV_CLICK);
}

bool MockDeviceImpl::click(const int x, const int y, const unsigned int intv)
{
    int seq = touchDown(x, y);
    if (seq < 0) return false;
    std::this_thread::sleep_for(std::chrono::milliseconds{intv});
    return touchUp(x, y, seq);
}

bool MockDeviceImpl::drag(const int sx, const int sy, const int ex, const int ey,
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

int MockDeviceImpl::touchDown(const int x, const int y)
{
    if (mTouchRelease.size() <= 0) return -1;
    int seq = mTouchRelease.begin()->first;
    mTouchDown[seq] = TouchData{x, y, timeStamp(), -1};
    mTouchRelease.erase(seq);
    return seq;
}

bool MockDeviceImpl::touchMove(const int x, const int y, const int seq)
{
    if (mTouchDown.count(seq) <= 0) return false;
    mTouchDown[seq] = TouchData{x, y, mTouchDown[seq].stamp1, timeStamp()};
    return true;
}

bool MockDeviceImpl::touchUp(const int x, const int y, const int seq)
{
    if (mTouchDown.count(seq) <= 0) return false;
    mTouchRelease[seq] = TouchData{x, y, mTouchDown[seq].stamp1, timeStamp()};
    mTouchDown.erase(seq);
    return true;
}

bool MockDeviceImpl::wheelUp(int amount, const int durationMs)
{
    for (int i = 0; i < amount; i++){
        mWheelDevice++;
        usleep(durationMs * MSEC_PER_SEC/amount);
    }

    return true;
}

bool MockDeviceImpl::wheelDown(int amount, const int durationMs)
{
    for (int i = 0; i < amount; i++){
        mWheelDevice--;
        usleep(durationMs * MSEC_PER_SEC/(double)amount);
    }

    return true;

}

bool MockDeviceImpl::pressBack(KeyRequestType type)
{
    mKeyDevice.push_back(std::tuple<MockKeyType, KeyRequestType, std::string>(MockKeyType::BACK, type, ""));
    return true;

}

bool MockDeviceImpl::pressHome(KeyRequestType type)
{
    mKeyDevice.push_back(std::tuple<MockKeyType, KeyRequestType, std::string>(MockKeyType::HOME, type, ""));
    return true;

}

bool MockDeviceImpl::pressMenu(KeyRequestType type)
{
    mKeyDevice.push_back(std::tuple<MockKeyType, KeyRequestType, std::string>(MockKeyType::MENU, type, ""));
    return true;
}

bool MockDeviceImpl::pressVolUp(KeyRequestType type)
{
    mKeyDevice.push_back(std::tuple<MockKeyType, KeyRequestType, std::string>(MockKeyType::VOLUP, type, ""));
    return true;
}

bool MockDeviceImpl::pressVolDown(KeyRequestType type)
{
    mKeyDevice.push_back(std::tuple<MockKeyType, KeyRequestType, std::string>(MockKeyType::VOLDOWN, type, ""));
    return true;
}

bool MockDeviceImpl::pressPower(KeyRequestType type)
{
    mKeyDevice.push_back(std::tuple<MockKeyType, KeyRequestType, std::string>(MockKeyType::POWER, type, ""));
    return true;
}

bool MockDeviceImpl::pressKeyCode(std::string keycode, KeyRequestType type)
{
    mKeyDevice.push_back(std::tuple<MockKeyType, KeyRequestType, std::string>(MockKeyType::KEY, type, keycode));
    return true;
}

bool MockDeviceImpl::takeScreenshot(std::string path, float scale, int quality)
{
    return true;
}

long long MockDeviceImpl::getSystemTime(TimeRequestType type)
{
    struct timespec t;

    if (type == TimeRequestType::MONOTONIC) {
        clock_gettime(CLOCK_MONOTONIC, &t);
    } else if (type == TimeRequestType::WALLCLOCK) {
        clock_gettime(CLOCK_REALTIME, &t);
    }

    return (long long)t.tv_sec * 1000L + (long long)(t.tv_nsec / MICRO_SEC);
}

bool MockDeviceImpl::strokeKeyCode(std::string keycode, unsigned int intv)
{
    return false;
}

bool MockDeviceImpl::pressKeyCode(std::string keycode)
{
    return false;
}

bool MockDeviceImpl::releaseKeyCode(std::string keycode)
{
    return false;
}

int MockDeviceImpl::grabTouchSeqNumber()
{
    return 0;
}

bool MockDeviceImpl::releaseTouchSeqNumber(int seq)
{
    return false;
}

void MockDeviceImpl::startTimer(void)
{
    isTimerStarted = true;
    clock_gettime(CLOCK_MONOTONIC, &tStart);
}

int MockDeviceImpl::stopTimer(void)
{
    struct timespec tEnd;
    if (!isTimerStarted) return 0;
    isTimerStarted = false;
    clock_gettime(CLOCK_MONOTONIC, &tEnd);
    return ((tEnd.tv_sec + tEnd.tv_nsec/NANO_SEC) - (tStart.tv_sec + tStart.tv_nsec/NANO_SEC)) * MICRO_SEC;
}

long long MockDeviceImpl::timeStamp(void)
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    long long rtn = (t.tv_sec + t.tv_nsec/NANO_SEC) * MICRO_SEC;
    return rtn;
}
