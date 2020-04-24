#ifndef IDEVICE_H
#define IDEVICE_H
#include "config.h"

#include <string>

enum class TypeRequestType {
    WALLCLOCK,
    MONOTONIC,
};

class IDevice {
public:
    virtual ~IDevice() {}

    virtual bool click(const int x, const int y) = 0;
    virtual bool click(const int x, const int y, const unsigned int intv) = 0;
    virtual bool drag(const int sx, const int sy, const int ex, const int ey,
                      const int steps) = 0;

    virtual bool touchDown(const int x, const int y) = 0;
    virtual bool touchMove(const int x, const int y) = 0;
    virtual bool touchUp(const int x, const int y) = 0;

    virtual bool pressBack() = 0;
    virtual bool pressHome() = 0;
    virtual bool pressMenu() = 0;
    virtual bool pressVolUp() = 0;
    virtual bool pressVolDown() = 0;
    virtual bool pressPower() = 0;
    virtual bool pressKeyCode(std::string keycode) = 0;

    virtual bool takeScreenshot(std::string path, float scale, int quality) = 0;
    virtual long long getSystemTime(TypeRequestType type) = 0;
};
#endif