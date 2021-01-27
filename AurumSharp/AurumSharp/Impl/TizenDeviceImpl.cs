using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading;
using ElmSharp;

namespace SharpTest.libsharptest
{
    internal class TizenDeviceImpl : IDevice
    {
        private IntPtr mFakeTouchHandle;

        private IntPtr mFakeKeyboardHandle;

        private IntPtr mFakeWheelHandle;

        private static readonly int INTV_CLICK = 5;

        private static readonly int INTV_SHORTSTROKE = 100;

        private static readonly int INTV_LONGSTROKE = 2000;

        private static readonly int INTV_MINIMUM_DRAG_MS = 25;

        private static readonly int INTV_MINIMUM_MSLEEP = 1;

        private static readonly int MINIMUM_DURATION_DRAG = 100;

        private static readonly int MSEC_PER_SEC = 1000;

        private static readonly int MAX_FINGER_NUMBER = 2;

        private Stopwatch timer = new Stopwatch();

        private SortedSet<int> mTouchSeq = new SortedSet<int>();

        public TizenDeviceImpl()
        {
            ElmSharp.EcoreMainloop.Send(() =>
            {
                mFakeTouchHandle = Interop.EflUtil.efl_util_input_initialize_generator(Interop.EflUtil.efl_util_input_device_type_e.EFL_UTIL_INPUT_DEVTYPE_TOUCHSCREEN);
                mFakeKeyboardHandle = Interop.EflUtil.efl_util_input_initialize_generator(Interop.EflUtil.efl_util_input_device_type_e.EFL_UTIL_INPUT_DEVTYPE_KEYBOARD);
                mFakeWheelHandle = Interop.EflUtil.efl_util_input_initialize_generator(Interop.EflUtil.efl_util_input_device_type_e.EFL_UTIL_INPUT_DEVTYPE_POINTER);
            });
        }

        ~TizenDeviceImpl()
        {
            ElmSharp.EcoreMainloop.Send(() =>
            {
                Interop.EflUtil.efl_util_input_deinitialize_generator(mFakeTouchHandle);
                Interop.EflUtil.efl_util_input_deinitialize_generator(mFakeKeyboardHandle);
                Interop.EflUtil.efl_util_input_deinitialize_generator(mFakeWheelHandle);
            });
        }

        public int TouchDown(int x, int y)
        {
            int seq = GrapTouchSeqNumber();
            if (seq >= 0)
            {
                ElmSharp.EcoreMainloop.Send(() =>
                {
                    Interop.EflUtil.efl_util_input_generate_touch(this.mFakeTouchHandle, seq, Interop.EflUtil.efl_util_input_touch_type_e.EFL_UTIL_INPUT_TOUCH_BEGIN, x, y);
                });
            }
            return seq;
        }

        private int GrapTouchSeqNumber()
        {
            for (int i = 0; i < MAX_FINGER_NUMBER; i++)
            {
                if (mTouchSeq.Contains(i) == false)
                {
                    mTouchSeq.Add(i);
                    return i;
                }
            }
            return -1;
        }
        private bool ReleaseTouchSeqNumber(int seq)
        {
            if(mTouchSeq.Contains(seq))
            {
                mTouchSeq.Remove(seq);
                return true;
            }
            return false;
        }

        public bool TouchMove(int x, int y, int seq)
        {
            bool result = false;
            if (seq >= 0)
            {
                ElmSharp.EcoreMainloop.Send(() =>
                {
                    result = Interop.EflUtil.efl_util_input_generate_touch(this.mFakeTouchHandle, seq, Interop.EflUtil.efl_util_input_touch_type_e.EFL_UTIL_INPUT_TOUCH_UPDATE, x, y) == 0;
                });
            }
            return result;
        }

        public bool TouchUp(int x, int y, int seq)
        {
            if (seq >= 0)
            {
                ElmSharp.EcoreMainloop.Send(() =>
                {
                    Interop.EflUtil.efl_util_input_generate_touch(this.mFakeTouchHandle, seq, Interop.EflUtil.efl_util_input_touch_type_e.EFL_UTIL_INPUT_TOUCH_END, x, y);
                });
            }
            return ReleaseTouchSeqNumber(seq);
        }

        public bool WheelDown(int amount, int durationMs)
        {
            bool result = false;
            ElmSharp.EcoreMainloop.Send(() =>
            {
                result = Interop.EflUtil.efl_util_input_generate_wheel(this.mFakeWheelHandle, Interop.EflUtil.efl_util_input_pointer_wheel_type_e.EFL_UTIL_INPUT_POINTER_WHEEL_HORZ, 1) == 0;
            });
            return result;
        }

        public bool WheelUp(int amount, int durationMs)
        {
            bool result = false;
            ElmSharp.EcoreMainloop.Send(() =>
            {
                result = Interop.EflUtil.efl_util_input_generate_wheel(this.mFakeWheelHandle, Interop.EflUtil.efl_util_input_pointer_wheel_type_e.EFL_UTIL_INPUT_POINTER_WHEEL_HORZ, -1) == 0;
            });
            return result;
        }

        public bool Click(int x, int y)
        {
            return Click(x, y, INTV_CLICK);
        }

        public bool Click(int x, int y, int intv)
        {
            int seq = TouchDown(x, y);
            if (seq < 0) return false;
            Thread.Sleep((int)intv);
            return TouchUp(x, y, seq);
        }

        public bool Drag(int sx, int sy, int ex, int ey, int steps, int durationMs)
        {
            int i, j;
            int _steps, _stepMs;
            int dur;
            int consumptionMs;

            if (steps <= 0) _steps = 1;
            else _steps = steps;

            if (durationMs < MINIMUM_DURATION_DRAG)
                durationMs = (int)(INTV_MINIMUM_DRAG_MS);

            _stepMs = durationMs / (_steps + 1);
            if (_stepMs < (INTV_MINIMUM_DRAG_MS))
                _steps = (int)(durationMs / (INTV_MINIMUM_DRAG_MS)) - 1;

            startTimer();
            int seq = TouchDown(sx, sy);

            if (seq < 0) return false;
            consumptionMs = stopTimer();

            for (int s = 1; s <= _steps + 1; s++)
            {
                Thread.Sleep((_stepMs - consumptionMs) > INTV_MINIMUM_MSLEEP ? (_stepMs - consumptionMs) : INTV_MINIMUM_MSLEEP);
                startTimer();
                TouchMove(sx + (ex - sx) * s / (steps + 1), sy + (ey - sy) * s / (steps + 1), seq);
                consumptionMs = stopTimer();
            }
            Thread.Sleep((_stepMs - consumptionMs) > INTV_MINIMUM_MSLEEP ? (_stepMs - consumptionMs) : INTV_MINIMUM_MSLEEP);
            TouchMove(ex, ey, seq);
            TouchUp(ex, ey, seq);

            return true;
        }

        private void startTimer()
        {
            timer.Start();
        }

        private int stopTimer()
        {
            if (timer.IsRunning == false) return 0;
            timer.Stop();
            if (timer.ElapsedMilliseconds > int.MaxValue) return 1000;
            return (int)timer.ElapsedMilliseconds;
        }

        public ulong GetSystemTime(TimeRequestType type)
        {
            throw new System.NotImplementedException();
        }

        public bool pressKeyCode(string keycode)
        {
            bool result = false;
            ElmSharp.EcoreMainloop.Send(() =>
            {
                 result = Interop.EflUtil.efl_util_input_generate_key(mFakeKeyboardHandle, keycode, 1) == 0;
            });

            return result; 
        }

        public bool releaseKeyCode(string keycode)
        {
            bool result = false;
            ElmSharp.EcoreMainloop.Send(() =>
            {
                result = Interop.EflUtil.efl_util_input_generate_key(mFakeKeyboardHandle, keycode, 0) == 0;
            });

            return result;
        }

        public bool strokeKeyCode(string keycode, int intv)
        {
            pressKeyCode(keycode);
            Thread.Sleep(intv);
            releaseKeyCode(keycode);
            return true;
        }

        public bool PressKeyCode(string keycode, KeyRequestType type)
        {
            if (type == KeyRequestType.STROKE)
                return strokeKeyCode(keycode, INTV_SHORTSTROKE);
            else if (type == KeyRequestType.LONG_STROKE)
                return strokeKeyCode(keycode, INTV_LONGSTROKE);
            else if (type == KeyRequestType.PRESS)
                return pressKeyCode(keycode);
            else if (type == KeyRequestType.RELEASE)
                return releaseKeyCode(keycode);
            return false;
        }

        public bool PressBack(KeyRequestType type)
        {
            return PressKeyCode("XF86Back", type);
        }

        public bool PressHome(KeyRequestType type)
        {
            return PressKeyCode("XF86Home", type);
        }

        public bool PressMenu(KeyRequestType type)
        {
            return PressKeyCode("XF86Menu", type);
        }

        public bool PressPower(KeyRequestType type)
        {
            return PressKeyCode("XF86PowerOff", type);
        }

        public bool PressVolDown(KeyRequestType type)
        {
            return PressKeyCode("XF86AudioLowerVolume", type);
        }

        public bool PressVolUp(KeyRequestType type)
        {
            return PressKeyCode("XF86AudioRaiseVolume", type);
        }

        public bool TakeScreenshot(string path, float scale, int quality)
        {
            throw new System.NotImplementedException();
        }
    }
}