using System;
using System.Runtime.InteropServices;

internal static partial class Interop
{
    internal static partial class Atspi
    {
        internal static partial class EventListener
        {
            public delegate void AtspiCallback(ref AtspiEvent atspihandle, IntPtr data);

            public delegate void GDtorCallback(IntPtr Data);

            [StructLayout(LayoutKind.Sequential)]
            public struct AtspiEvent
            {
                public string type;

                public IntPtr source;

                public int detail1;

                public int detail2;

                [MarshalAs(UnmanagedType.ByValArray, SizeConst = 6)]
                public int[] any_data;
            }

            [DllImport("libatspi.so.0")]
            internal static extern IntPtr atspi_event_listener_new(AtspiCallback callback,IntPtr data, GDtorCallback dtor_callback);

            [DllImport("libatspi.so.0")]
            internal static extern int atspi_event_listener_register(IntPtr handle, String key, IntPtr cb);

            [DllImport("libatspi.so.0")]
            internal static extern int atspi_event_listener_deregister(IntPtr handle, String key, out IntPtr err);
        }
    }
}
