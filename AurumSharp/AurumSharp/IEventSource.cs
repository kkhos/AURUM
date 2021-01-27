using System;
using System.Collections.Generic;
using System.Text;

namespace SharpTest.libsharptest
{
    public interface IEventSource
    {
        void attach(IEventConsumer source);
        void detach(IEventConsumer source);
        void notifyAll(int type, int type2, IntPtr src);
    }
}
