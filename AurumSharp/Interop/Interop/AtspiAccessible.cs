using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

internal static partial class Interop
{
    internal static partial class Atspi
    {
        internal static partial class Accessible
        {
            [DllImport("libatspi.so.0")]
            internal static extern string atspi_role_get_name(int role);

            [DllImport("libatspi.so.0")]
            internal static extern string atspi_accessible_get_name(IntPtr atspiaccessibleObj, out IntPtr error);

            [DllImport("libatspi.so.0")]
            internal static extern string atspi_accessible_get_description(IntPtr atspiAccessibleObj, out IntPtr error);

            [DllImport("libatspi.so.0")]
            internal static extern IntPtr atspi_accessible_get_parent(IntPtr atspiAccessibleObj, out IntPtr error);

            [DllImport("libatspi.so.0")]
            internal static extern int atspi_accessible_get_child_count(IntPtr atspiAccessibleObj, out IntPtr error);

            [DllImport("libatspi.so.0")]
            internal static extern IntPtr atspi_accessible_get_child_at_index(IntPtr atspiAccessibleObj, int child_index, out IntPtr error);

            [DllImport("libatspi.so.0")]
            internal static extern int atspi_accessible_get_index_in_parent(IntPtr atspiAccessibleObj, out IntPtr error);

            [DllImport("libatspi.so.0")]
            internal static extern IntPtr atspi_accessible_get_relation_set(IntPtr atspiAccessibleObj, out IntPtr error);

            [DllImport("libatspi.so.0")]
            internal static extern int atspi_accessible_get_role(IntPtr atspiAccessibleObj, out IntPtr error);

            [DllImport("libatspi.so.0")]
            internal static extern string atspi_accessible_get_role_name(IntPtr atspiAccessibleObj, out IntPtr error);

            [DllImport("libatspi.so.0")]
            internal static extern string atspi_accessible_get_unique_id(IntPtr atspiAccessibleObj, out IntPtr error);
            
            [DllImport("libatspi.so.0")]
            internal static extern string atspi_accessible_get_localized_role_name(IntPtr atspiAccessibleObj, out IntPtr error);

            [DllImport("libatspi.so.0")]
            internal static extern IntPtr atspi_accessible_get_state_set(IntPtr atspiAccessibleObj);

            [DllImport("libatspi.so.0")]
            internal static extern IntPtr atspi_accessible_get_attributes(IntPtr atspiAccessibleObj, out IntPtr error);

            [DllImport("libatspi.so.0")]
            internal static extern IntPtr atspi_accessible_get_attributes_as_array(IntPtr atspiAccessibleObj, out IntPtr error);

            [DllImport("libatspi.so.0")]
            internal static extern string atspi_accessible_get_toolkit_name(IntPtr atspiAccessibleObj, out IntPtr error);

            [DllImport("libatspi.so.0")]
            internal static extern string atspi_accessible_get_toolkit_version(IntPtr atspiAccessibleObj, out IntPtr error);

            [DllImport("libatspi.so.0")]
            internal static extern IntPtr atspi_accessible_get_application(IntPtr atspiAccessibleObj, out IntPtr error);

            [DllImport("libatspi.so.0")]
            internal static extern IntPtr atspi_accessible_get_action(IntPtr atspiAccessibleObj);

            [DllImport("libatspi.so.0")]
            internal static extern IntPtr atspi_accessible_get_action_iface(IntPtr atspiAccessibleObj);

            [DllImport("libatspi.so.0")]
            internal static extern IntPtr atspi_accessible_get_collection(IntPtr atspiAccessibleObj);

            [DllImport("libatspi.so.0")]
            internal static extern IntPtr atspi_accessible_get_component(IntPtr atspiAccessibleObj);

            [DllImport("libatspi.so.0")]
            internal static extern IntPtr atspi_accessible_get_component_iface(IntPtr atspiAccessibleObj);

            [DllImport("libatspi.so.0")]
            internal static extern IntPtr atspi_accessible_get_document(IntPtr atspiAccessibleObj);

            [DllImport("libatspi.so.0")]
            internal static extern IntPtr atspi_accessible_get_editable_text(IntPtr atspiAccessibleObj);

            [DllImport("libatspi.so.0")]
            internal static extern IntPtr atspi_accessible_get_hyperlink(IntPtr atspiAccessibleObj);

            [DllImport("libatspi.so.0")]
            internal static extern IntPtr atspi_accessible_get_hypertext(IntPtr atspiAccessibleObj);

            [DllImport("libatspi.so.0")]
            internal static extern IntPtr atspi_accessible_get_image(IntPtr atspiAccessibleObj);

            [DllImport("libatspi.so.0")]
            internal static extern IntPtr atspi_accessible_get_selection(IntPtr atspiAccessibleObj);

            [DllImport("libatspi.so.0")]
            internal static extern IntPtr atspi_accessible_get_table(IntPtr atspiAccessibleObj);

            [DllImport("libatspi.so.0")]
            internal static extern IntPtr atspi_accessible_get_text(IntPtr atspiAccessibleObj);

            [DllImport("libatspi.so.0")]
            internal static extern IntPtr atspi_accessible_get_value(IntPtr atspiAccessibleObj);

            [DllImport("libatspi.so.0")]
            internal static extern IntPtr atspi_accessible_get_interfaces(IntPtr atspiAccessibleObj);
        }
    }
}


