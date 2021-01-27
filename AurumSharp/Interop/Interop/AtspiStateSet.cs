using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

internal static partial class Interop
{
    internal static partial class Atspi
    {
        public enum AtspiStateType
        {
            ATSPI_STATE_INVALID,
            ATSPI_STATE_ACTIVE,
            ATSPI_STATE_ARMED,
            ATSPI_STATE_BUSY,
            ATSPI_STATE_CHECKED,
            ATSPI_STATE_COLLAPSED,
            ATSPI_STATE_DEFUNCT,
            ATSPI_STATE_EDITABLE,
            ATSPI_STATE_ENABLED,
            ATSPI_STATE_EXPANDABLE,
            ATSPI_STATE_EXPANDED,
            ATSPI_STATE_FOCUSABLE,
            ATSPI_STATE_FOCUSED,
            ATSPI_STATE_HAS_TOOLTIP,
            ATSPI_STATE_HORIZONTAL,
            ATSPI_STATE_ICONIFIED,
            ATSPI_STATE_MODAL,
            ATSPI_STATE_MULTI_LINE,
            ATSPI_STATE_MULTISELECTABLE,
            ATSPI_STATE_OPAQUE,
            ATSPI_STATE_PRESSED,
            ATSPI_STATE_RESIZABLE,
            ATSPI_STATE_SELECTABLE,
            ATSPI_STATE_SELECTED,
            ATSPI_STATE_SENSITIVE,
            ATSPI_STATE_SHOWING,
            ATSPI_STATE_SINGLE_LINE,
            ATSPI_STATE_STALE,
            ATSPI_STATE_TRANSIENT,
            ATSPI_STATE_VERTICAL,
            ATSPI_STATE_VISIBLE,
            ATSPI_STATE_MANAGES_DESCENDANTS,
            ATSPI_STATE_INDETERMINATE,
            ATSPI_STATE_REQUIRED,
            ATSPI_STATE_TRUNCATED,
            ATSPI_STATE_ANIMATED,
            ATSPI_STATE_INVALID_ENTRY,
            ATSPI_STATE_SUPPORTS_AUTOCOMPLETION,
            ATSPI_STATE_SELECTABLE_TEXT,
            ATSPI_STATE_IS_DEFAULT,
            ATSPI_STATE_VISITED,
            ATSPI_STATE_CHECKABLE,
            ATSPI_STATE_HAS_POPUP,
            ATSPI_STATE_READ_ONLY,
            ATSPI_STATE_HIGHLIGHTED,
            ATSPI_STATE_HIGHLIGHTABLE,
            ATSPI_STATE_LAST_DEFINED,
        }

        internal static partial class StateSet
        {
            [DllImport("libatspi.so.0")]
            internal static extern int atspi_state_set_contains(IntPtr handle, AtspiStateType state);

            [DllImport("libatspi.so.0")]
            internal static extern IntPtr atspi_state_set_get_states(IntPtr handle);
        }
    }
}


