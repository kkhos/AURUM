using System;
using System.Runtime.InteropServices;

internal static partial class Interop
{
    internal static partial class Atspi
    {
        internal static partial class Action
        {
            [DllImport("libatspi.so.0")]
            internal static extern int atspi_action_get_n_actions(IntPtr obj, out IntPtr error);

            [DllImport("libatspi.so.0")]
            internal static extern string atspi_action_get_description(IntPtr obj, int i, out IntPtr error);

            [DllImport("libatspi.so.0")]
            internal static extern string atspi_action_get_key_binding(IntPtr obj, int i, out IntPtr error);

            [DllImport("libatspi.so.0")]
            internal static extern string atspi_action_get_name(IntPtr obj, int i, out IntPtr error);

            [DllImport("libatspi.so.0")]
            internal static extern string atspi_action_get_action_name(IntPtr obj, int i, out IntPtr error);
            
            [DllImport("libatspi.so.0")]
            internal static extern int atspi_action_do_action(IntPtr obj, int i, out IntPtr error);
        }
    }
}


