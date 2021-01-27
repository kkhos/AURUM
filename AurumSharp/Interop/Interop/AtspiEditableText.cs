using System;
using System.Runtime.InteropServices;

internal static partial class Interop
{
    internal static partial class Atspi
    {
        internal static partial class EditableText
        {
            [DllImport("libatspi.so.0")]
            internal static extern int atspi_editable_text_delete_text(IntPtr obj, int start_pos, int end_pos, out IntPtr error);

            [DllImport("libatspi.so.0")]
            internal static extern int atspi_editable_text_insert_text(IntPtr obj, int position, string text, int length, out IntPtr error);
        }
    }
}


