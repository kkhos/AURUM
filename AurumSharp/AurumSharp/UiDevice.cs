using System;
using System.Collections.Generic;
using System.Text;

namespace SharpTest.libsharptest
{
    public class UiDevice : IDevice, ISearchable
    {
        private IDevice mDeviceImpl { set; get; }

        private static UiDevice instance = null;


        private UiDevice()
        {
            AccessibleWatcher.Instance();
        }

        private UiDevice(IDevice impl)
        {
            AccessibleWatcher.Instance();
            mDeviceImpl = impl;
        }

        public static UiDevice Instance(IDevice deviceImpl = null)
        {
            if (deviceImpl != null)
            {
                instance = new UiDevice(deviceImpl);
            }
            else
            {
                if (instance == null) instance = new UiDevice(new TizenDeviceImpl());
            }

            return instance;
        }

        List<AccessibleNode> getWindowRoot()
        {
            List<AccessibleNode> ret = new List<AccessibleNode>();
            List<AccessibleApplication> apps = AccessibleWatcher.Instance().getActiveApplications();

            foreach (var app in apps)
            {
                List<AccessibleWindow> wins = app.getActiveWindows();
                foreach (var win in wins)
                   ret.Add(win.getAccessibleNode());
            }
            return ret;
        }

        public UiObject FindObject(UiSelector selector)
        {
            var rootNodes = getWindowRoot();
            foreach (var node in rootNodes)
            {
                AccessibleNode found = Comparer.findObject(Instance(), selector, node);
                if (found != null) return new UiObject(Instance(), selector, found);
            }

            return null;
        }

        public List<UiObject> FindObjects(UiSelector selector)
        {
            List<UiObject> ret = new List<UiObject>();

            var rootWins = getWindowRoot();

            foreach (var win in rootWins)
            {
                List<AccessibleNode> nodes = Comparer.findObjects(Instance(), selector, win);
                foreach (var node in nodes)
                {
                    ret.Add(new UiObject(Instance(), selector, node));
                }
            }
            return ret;
        }

        public bool HasObject(UiSelector selector)
        {
            UiObject found = FindObject(selector);
            if (found != null) return true;
            return false;
        }

        private bool WaitForIdle()
        {
            System.Threading.Thread.Sleep(167);
            return true;
        }

        public bool Click(int x, int y)
        {
            bool result = mDeviceImpl.Click(x, y);
            WaitForIdle();    
            return result;
        }

        public bool Click(int x, int y, int intv)
        {
            bool result = mDeviceImpl.Click(x, y, intv);
            WaitForIdle();
            return result;
        }

        public bool Drag(int sx, int sy, int ex, int ey, int steps, int durationMs)
        {
            bool result = mDeviceImpl.Drag(sx, sy, ex, ey, steps, durationMs);
            WaitForIdle();
            return result;
        }

        public ulong GetSystemTime(TimeRequestType type)
        {
            return mDeviceImpl.GetSystemTime(type);
        }

        public bool PressBack(KeyRequestType type)
        {
            bool result = mDeviceImpl.PressBack(type);
            WaitForIdle();
            return result;
        }

        public bool PressHome(KeyRequestType type)
        {
            bool result = mDeviceImpl.PressHome(type);
            WaitForIdle();
            return result;
        }

        public bool PressKeyCode(string keycode, KeyRequestType type)
        {
            bool result = mDeviceImpl.PressKeyCode(keycode, type);
            WaitForIdle();
            return result;
        }

        public bool PressMenu(KeyRequestType type)
        {
            bool result = mDeviceImpl.PressMenu(type);
            WaitForIdle();
            return result;
        }

        public bool PressPower(KeyRequestType type)
        {
            bool result = mDeviceImpl.PressPower(type);
            WaitForIdle();
            return result;
        }

        public bool PressVolDown(KeyRequestType type)
        {
            bool result = mDeviceImpl.PressVolDown(type);
            WaitForIdle();
            return result;
        }

        public bool PressVolUp(KeyRequestType type)
        {
            bool result = mDeviceImpl.PressVolUp(type);
            WaitForIdle();
            return result;
        }

        public bool TakeScreenshot(string path, float scale, int quality)
        {
            bool result = mDeviceImpl.TakeScreenshot(path, scale, quality);
            WaitForIdle();
            return result;
        }

        public int TouchDown(int x, int y)
        {
            int result = mDeviceImpl.TouchDown(x, y);
            WaitForIdle();
            return result;
        }

        public bool TouchMove(int x, int y, int seq)
        {
            bool result = mDeviceImpl.TouchMove(x, y, seq);
            WaitForIdle();
            return result;
        }

        public bool TouchUp(int x, int y, int seq)
        {
            bool result = mDeviceImpl.TouchUp(x, y, seq);
            WaitForIdle();
            return result;
        }

        public bool WheelDown(int amount, int durationMs)
        {
            bool result = mDeviceImpl.WheelDown(amount, durationMs);
            WaitForIdle();
            return result;
        }

        public bool WheelUp(int amount, int durationMs)
        {
            bool result = mDeviceImpl.WheelUp(amount, durationMs);
            WaitForIdle();
            return result;
        }
    }
}
