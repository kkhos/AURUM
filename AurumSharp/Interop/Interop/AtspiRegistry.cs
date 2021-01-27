using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

internal static partial class Interop
{
    internal static partial class Atspi
    {
        internal static partial class Registry
        {
            [DllImport("libatspi.so.0")]
            internal static extern int atspi_get_desktop_count();

            [DllImport("libatspi.so.0")]
            internal static extern IntPtr atspi_get_desktop(int i);

            [DllImport("libatspi.so.0")]
            internal static extern IntPtr atspi_get_desktop_list();
            /*
            [DllImport("libatspi.so.0")]
            GType	atspi_key_definition_get_type ();

            [DllImport("libatspi.so.0")]
            int atspi_register_keystroke_listener ();

            [DllImport("libatspi.so.0")]
            int atspi_deregister_keystroke_listener ();

            [DllImport("libatspi.so.0")]
            int atspi_register_device_event_listener ();

            [DllImport("libatspi.so.0")]
            int atspi_deregister_device_event_listener ();

            [DllImport("libatspi.so.0")]
            int atspi_generate_keyboard_event ();

            [DllImport("libatspi.so.0")]
            int atspi_generate_mouse_event ();
            */
        }
    }
}


