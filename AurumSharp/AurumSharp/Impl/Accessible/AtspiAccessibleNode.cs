using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace SharpTest.libsharptest.Impl.Accessible
{
    class AtspiAccessibleNode : AccessibleNode
    {
        private IntPtr mNode = IntPtr.Zero;

        public AtspiAccessibleNode(IntPtr node)
        {
            this.mNode = node;
            if (node != IntPtr.Zero)
            {
                refresh();
                var watcher = AccessibleWatcher.Instance();
                watcher.attach(this);
            }
        }

        ~AtspiAccessibleNode()
        {
            if (mNode != IntPtr.Zero)
            {
                var watcher = AccessibleWatcher.Instance();
                watcher.detach(this);
                Interop.Gobject.g_object_unref(mNode);
            }
        }

        public override bool doAction(string actionName)
        {
            IntPtr action, error ;
            if (!isValid()) return false;

            action = Interop.Atspi.Accessible.atspi_accessible_get_action_iface(mNode);
            if (action != IntPtr.Zero)
            {
                int a;
                int n_actions = Interop.Atspi.Action.atspi_action_get_n_actions(action, out error);

                for (a = 0; a < n_actions; a++)
                {
                    string action_name = Interop.Atspi.Action.atspi_action_get_action_name(action, a, out error);
                    if (action_name == null || action_name.Length <= 0)
                    {
                        Interop.Gobject.g_object_unref(action);
                        return false;
                    }
                    
                    if (actionName.CompareTo(action_name) == 0)
                    {
                        Interop.Atspi.Action.atspi_action_do_action(action, a, out error);
                        Interop.Gobject.g_object_unref(action);
                        return true;
                    }
                }
                Interop.Gobject.g_object_unref(action);
            }
            return false;
        }

        public override List<string> getActions()
        {
            List<string> result = new List<string>();
            IntPtr action, error;
            if (!isValid()) return result;

            action = Interop.Atspi.Accessible.atspi_accessible_get_action_iface(mNode);
            if (action != IntPtr.Zero)
            {
                int i;
                int n_actions = Interop.Atspi.Action.atspi_action_get_n_actions(action, out error);

                for (i = 0; i < n_actions; i++)
                {
                    string action_name = Interop.Atspi.Action.atspi_action_get_action_name(action, i, out error);
                    if (action_name == null || action_name.Length < 0) continue;
                    result.Add(action_name);
                }
                Interop.Gobject.g_object_unref(action);
            }
            return result;
        }

        public override AccessibleNode getChildAt(int index)
        {
            IntPtr error;
            if (!isValid()) return new AtspiAccessibleNode(IntPtr.Zero);
            IntPtr rawChild = Interop.Atspi.Accessible.atspi_accessible_get_child_at_index(mNode, index, out error);
            return new AtspiAccessibleNode(rawChild);
        }

        public override int getChildCount()
        {
            IntPtr error;
            //Tizen.Log.Info("WKWK", "WKWK child count 1");
            if (!isValid()) return 0;
            //Tizen.Log.Info("WKWK", "WKWK child count 2");
            int count = Interop.Atspi.Accessible.atspi_accessible_get_child_count(mNode, out error);
            //Tizen.Log.Info("WKWK", "WKWK child count 3 " + count.ToString() + " " + mNode.ToString());
            if (count <= 0) return 0;
            //Tizen.Log.Info("WKWK", "WKWK child count 4");
            return count;
        }   

        public override List<AccessibleNode> getChildren()
        {
            List<AccessibleNode> ret = new List<AccessibleNode>();
            int nchild = getChildCount();
            for (int i = 0; i < nchild; i++)
            {
                var child = getChildAt(i);
                if (child.isValid()) ret.Add(child);
            }
            return ret;
        }

        public override AccessibleNode getParent()
        {
            IntPtr error;
            if (!isValid()) return new AtspiAccessibleNode(IntPtr.Zero);
            IntPtr rawParent = Interop.Atspi.Accessible.atspi_accessible_get_parent(mNode, out error);
            return new AtspiAccessibleNode(rawParent);
        }

        public override IntPtr getRawhandler()
        {
            return mNode;
        }

        private void setFeatureProperty(Interop.Atspi.AtspiStateType type)
        {
            /*
                LONGCLICKABLE = 0X0040,
                SCROLLABLE = 0X0080,
            */
            switch (type)
            {
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_CHECKED:
                    setFeatureProperty(NodeFeatureProperties.CHECKED, true);
                    break;
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_CHECKABLE:
                    setFeatureProperty(NodeFeatureProperties.CHECKABLE, true);
                    break;
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_ENABLED:
                    setFeatureProperty(NodeFeatureProperties.ENABLED, true);
                    break;
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_FOCUSABLE:
                    setFeatureProperty(NodeFeatureProperties.FOCUSABLE, true);
                    break;
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_FOCUSED:
                    setFeatureProperty(NodeFeatureProperties.FOCUSED, true);
                    break;
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_SELECTABLE:
                    setFeatureProperty(NodeFeatureProperties.SELECTABLE, true);
                    break;
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_SELECTED:
                    setFeatureProperty(NodeFeatureProperties.SELECTED, true);
                    break;
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_SHOWING:
                    setFeatureProperty(NodeFeatureProperties.SHOWING, true);
                    break;
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_VISIBLE:
                    setFeatureProperty(NodeFeatureProperties.VISIBLE, true);
                    break;
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_ACTIVE:
                    setFeatureProperty(NodeFeatureProperties.ACTIVE, true);
                    break;
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_SENSITIVE:
                    setFeatureProperty(NodeFeatureProperties.CLICKABLE, true);
                    break;
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_DEFUNCT:
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_INVALID:
                    setFeatureProperty(NodeFeatureProperties.INVALID, true);
                    break;
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_TRANSIENT:
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_TRUNCATED:
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_ANIMATED:
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_ARMED:
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_BUSY:
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_COLLAPSED:
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_EDITABLE:
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_EXPANDABLE:
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_EXPANDED:
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_HAS_TOOLTIP:
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_HORIZONTAL:
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_ICONIFIED:
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_MODAL:
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_MULTI_LINE:
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_MULTISELECTABLE:
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_OPAQUE:
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_PRESSED:
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_RESIZABLE:
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_SINGLE_LINE:
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_STALE:
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_VERTICAL:
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_MANAGES_DESCENDANTS:
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_INDETERMINATE:
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_REQUIRED:
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_INVALID_ENTRY:
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_SUPPORTS_AUTOCOMPLETION:
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_SELECTABLE_TEXT:
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_IS_DEFAULT:
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_VISITED:
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_HAS_POPUP:
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_READ_ONLY:
                case Interop.Atspi.AtspiStateType.ATSPI_STATE_LAST_DEFINED:
                default:
                    break;
            }
        }

        public override void refresh()
        {
            IntPtr error;
            if (isValid())
            {
                string rolename = Interop.Atspi.Accessible.atspi_accessible_get_role_name(mNode, out error);
                if (rolename != null && rolename.Length > 0)
                {
                    mRole = "" + rolename;
                }
                string uID = Interop.Atspi.Accessible.atspi_accessible_get_unique_id(mNode, out error);
                if (uID != null && uID.Length > 0)
                {
                    mId = "" + uID;
                }
                mText = mPkg = Interop.Atspi.Accessible.atspi_accessible_get_name(mNode, out error); ;

                
                IntPtr attributes = Interop.Atspi.Accessible.atspi_accessible_get_attributes(mNode, out error);
                if (attributes != IntPtr.Zero)
                {
                    mType = Marshal.PtrToStringAnsi(Interop.Ghashtable.g_hash_table_lookup(attributes, "type"));
                    mStyle = Marshal.PtrToStringAnsi(Interop.Ghashtable.g_hash_table_lookup(attributes, "style"));
                    mAutomationId = Marshal.PtrToStringAnsi(Interop.Ghashtable.g_hash_table_lookup(attributes, "automationId"));
                    Interop.Ghashtable.g_hash_table_unref(attributes);
                }
                IntPtr st = Interop.Atspi.Accessible.atspi_accessible_get_state_set(mNode);
                if (st != IntPtr.Zero)
                {
                    IntPtr states = Interop.Atspi.StateSet.atspi_state_set_get_states(st);
                    Interop.Glib.GArray array = (Interop.Glib.GArray)Marshal.PtrToStructure(states, typeof(Interop.Glib.GArray));

                    int[] statsArray = new int[array.len];
                    Marshal.Copy(array.data, statsArray, 0, (int)array.len);


                    if (states != IntPtr.Zero)
                    {
                        Interop.Atspi.AtspiStateType stat;
                        for (uint i = 0; states != IntPtr.Zero && (i < array.len); ++i)
                        {
                            stat = (Interop.Atspi.AtspiStateType) statsArray[i];
                            setFeatureProperty(stat);
                        }

                        Interop.Glib.g_array_free(states, 1);
                    }
                    Interop.Gobject.g_object_unref(st);
                }
                
                IntPtr component = Interop.Atspi.Accessible.atspi_accessible_get_component_iface(mNode);
                if (component != IntPtr.Zero)
                {
                    IntPtr extent = Interop.Atspi.Component.atspi_component_get_extents(component, Interop.Atspi.Component.AtspiCoordType.ATSPI_COORD_TYPE_SCREEN, out error);
                    if (extent != IntPtr.Zero)
                    {
                        Interop.Atspi.Component.AtspiRect extent_ = (Interop.Atspi.Component.AtspiRect)Marshal.PtrToStructure(extent, typeof(Interop.Atspi.Component.AtspiRect));
                        mBoundingBox = new Rect(extent_.x, extent_.y, extent_.x + extent_.width, extent_.y + extent_.height);
                        Interop.Glib.g_free(extent);
                    }
                    Interop.Gobject.g_object_unref(component);
                }
            }
            else
            {
                setFeatureProperty(Interop.Atspi.AtspiStateType.ATSPI_STATE_INVALID);
            }
        }

        public override void setText(string text)
        {
            IntPtr error;
            if (!isValid()) return;

            IntPtr iface = Interop.Atspi.Accessible.atspi_accessible_get_editable_text(mNode);
            //LOG_F(INFO, "set Value iface:%p obj:%p text:%s", iface, mNode, text.c_str());
            if (iface != IntPtr.Zero)
            {
                int len = getText().Length;
                Interop.Atspi.EditableText.atspi_editable_text_delete_text(iface, 0, len, out error);
                Interop.Atspi.EditableText.atspi_editable_text_insert_text(iface, 0, text, text.Length, out error);
            }
    }
    }
}
