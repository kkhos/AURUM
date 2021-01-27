using System;
using System.Runtime.InteropServices;

internal static partial class Interop
{
    internal static class Ghashtable
    {
        [DllImport("libglib-2.0.so.0")]
        internal static extern IntPtr g_hash_table_lookup(IntPtr hash_table, string key);

        [DllImport("libglib-2.0.so.0")]
        internal static extern void g_hash_table_unref(IntPtr hash_table);
    }
}
