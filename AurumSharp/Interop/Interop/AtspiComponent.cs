using System;
using System.Runtime.InteropServices;

internal static partial class Interop
{
    internal static partial class Atspi
    {

        internal static partial class Component
        {
            public struct AtspiRect
            {
                public int x;
                public int y;
                public int width;
                public int height;
            };

            public enum AtspiCoordType {
                ATSPI_COORD_TYPE_SCREEN,
                ATSPI_COORD_TYPE_WINDOW,
            }

            [DllImport("libatspi.so.0")]
            internal static extern IntPtr atspi_component_get_extents(IntPtr obj, AtspiCoordType ctype, out IntPtr error);
        }
    }
}


