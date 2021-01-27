using System;
using System.Collections.Generic;
using System.Text;

namespace SharpTest.libsharptest
{
    enum EventType
    {
        Window,
        Object,
};

    enum WindowEventType
    {
        WindowActivated,
        WindowDeactivated,
        WindowCreated,
        WindowDestroyed,
};

    enum ObjectEventType
    {
        ObjectStateVisible,
        ObjectStateDefunct,
};

    public interface IEventConsumer
    {
        void notify(int type, int type2, IntPtr src);
    }
}
