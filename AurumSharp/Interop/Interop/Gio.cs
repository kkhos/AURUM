using System;
using System.Runtime.InteropServices;

internal static partial class Interop
{
    internal static class Gio
    {
        [DllImport("libgio-2.0.so.0")]
        internal static extern IntPtr g_dbus_proxy_new_for_bus_sync(int a, int b, IntPtr info, string c, string d, string e, IntPtr aa, out IntPtr error);

        [DllImport("libgio-2.0.so.0")]
        internal static extern IntPtr g_variant_new_boolean(int b);

        [DllImport("libgio-2.0.so.0")]
        internal static extern IntPtr g_dbus_proxy_call_sync(IntPtr proxy, string a, IntPtr var, int aa, int bb, IntPtr b, out IntPtr error);

        [DllImport("libgio-2.0.so.0", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr g_variant_new(string a, string b, string c, IntPtr d);

        [DllImport("libgio-2.0.so.0")]
        internal static extern void g_variant_unref(IntPtr value);
    }
}
