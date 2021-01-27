using System;
using System.Collections.Generic;
using System.Text;

namespace SharpTest.libsharptest
{
    public enum TimeRequestType
    {
        WALLCLOCK,
        MONOTONIC,
    };

    public enum KeyRequestType
    {
        STROKE,
        LONG_STROKE,
        PRESS,
        RELEASE,
    };

    public interface IDevice
    {
        bool Click(int x, int y);

        bool Click(int x, int y, int intv);

        bool Drag(int sx, int sy, int ex, int ey, int steps, int durationMs);

        int TouchDown(int x, int y);

        bool TouchMove(int x, int y, int seq);

        bool TouchUp(int x, int y, int seq);

        bool WheelUp(int amount, int durationMs);

        bool WheelDown(int amount, int durationMs);

        bool PressBack(KeyRequestType type);

        bool PressHome(KeyRequestType type);

        bool PressMenu(KeyRequestType type);

        bool PressVolUp(KeyRequestType type);

        bool PressVolDown(KeyRequestType type);

        bool PressPower(KeyRequestType type);

        bool PressKeyCode(String keycode, KeyRequestType type);

        bool TakeScreenshot(String path, float scale, int quality);

        ulong GetSystemTime(TimeRequestType type);
    }
}
