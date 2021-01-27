using System;
using System.Runtime.InteropServices;

internal static partial class Interop
{
    internal static partial class Atspi
    {
        internal static partial class Misc
        {
            [DllImport("libatspi.so.0")]
            internal static extern int atspi_init();

            [DllImport("libatspi.so.0")]
            internal static extern void atspi_event_main();

            [DllImport("libatspi.so.0")]
            internal static extern void atspi_event_quit();

            [DllImport("libatspi.so.0")]
            internal static extern int atspi_exit();

            [DllImport("libatspi.so.0")]
            internal static extern void atspi_set_main_context(IntPtr ctx);
        }
    }
}


