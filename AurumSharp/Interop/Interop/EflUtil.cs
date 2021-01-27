using System;
using System.Collections.Generic;
using System.Text;

using System.Runtime.InteropServices;

internal static partial class Interop
{
    internal static class EflUtil
    {
        internal enum efl_util_input_device_type_e
        {
            EFL_UTIL_INPUT_DEVTYPE_NONE = 0x0,
            EFL_UTIL_INPUT_DEVTYPE_TOUCHSCREEN = (1 << 0),
            EFL_UTIL_INPUT_DEVTYPE_KEYBOARD = (1 << 1),
            EFL_UTIL_INPUT_DEVTYPE_POINTER = (1 << 2),
            EFL_UTIL_INPUT_DEVTYPE_ALL = EFL_UTIL_INPUT_DEVTYPE_TOUCHSCREEN |
                                         EFL_UTIL_INPUT_DEVTYPE_KEYBOARD, 
            EFL_UTIL_INPUT_DEVTYPE_MAX = (1 << 10)
        }

        internal enum efl_util_input_touch_type_e
        {
            EFL_UTIL_INPUT_TOUCH_NONE, 
            EFL_UTIL_INPUT_TOUCH_BEGIN, 
            EFL_UTIL_INPUT_TOUCH_UPDATE,
            EFL_UTIL_INPUT_TOUCH_END,
            EFL_UTIL_INPUT_TOUCH_MAX = 10 
        }

        internal enum efl_util_input_pointer_type_e
        {
            EFL_UTIL_INPUT_POINTER_BUTTON_DOWN,
            EFL_UTIL_INPUT_POINTER_BUTTON_UP,
            EFL_UTIL_INPUT_POINTER_MOVE,
        }; 
 
        internal enum efl_util_input_pointer_wheel_type_e
        {
            EFL_UTIL_INPUT_POINTER_WHEEL_VERT,
            EFL_UTIL_INPUT_POINTER_WHEEL_HORZ,
        };

        [DllImport("libcapi-ui-efl-util.so.0")]
        internal static extern IntPtr efl_util_input_initialize_generator(efl_util_input_device_type_e type);

        [DllImport("libcapi-ui-efl-util.so.0")]
        internal static extern int efl_util_input_deinitialize_generator(IntPtr handle);

        [DllImport("libcapi-ui-efl-util.so.0")]
        internal static extern int efl_util_input_generate_touch(IntPtr handle, int seq, efl_util_input_touch_type_e type, int x, int y);

        [DllImport("libcapi-ui-efl-util.so.0")]
        internal static extern int efl_util_input_generate_wheel(IntPtr handle, efl_util_input_pointer_wheel_type_e type, int amount);

        [DllImport("libcapi-ui-efl-util.so.0")]
        internal static extern int efl_util_input_generate_pointer(IntPtr handle, int buttons, efl_util_input_pointer_type_e type, int x, int y);

        [DllImport("libcapi-ui-efl-util.so.0")]
        internal static extern int efl_util_input_generate_key(IntPtr handle, String key, int type);
    }
}


