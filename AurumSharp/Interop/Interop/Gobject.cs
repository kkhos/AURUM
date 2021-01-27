using System;
using System.Runtime.InteropServices;

internal static partial class Interop
{
    internal static class Gobject
    {
        [DllImport("libgobject-2.0.so.0")]
        internal static extern void g_object_unref(IntPtr obj);
    }
}
