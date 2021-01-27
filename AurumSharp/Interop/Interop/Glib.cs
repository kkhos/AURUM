using System;
using System.Runtime.InteropServices;

internal static partial class Interop
{
    internal static class Glib
    {
        public struct GArray
        {
            public IntPtr data;
            public uint len;
        }

        [DllImport("libglib-2.0.so.0")]
        internal static extern string g_hash_table_lookup(IntPtr hash_table, string key);

        [DllImport("libglib-2.0.so.0")]
        internal static extern IntPtr g_array_free(IntPtr array, int free_segment);

        [DllImport("libglib-2.0.so.0")]
        internal static extern void g_free(IntPtr mem);

        [DllImport("libglib-2.0.so.0")]
        internal static extern IntPtr g_main_context_default();

    }
    
}
