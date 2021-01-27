using System;
using System.Collections.Generic;
using System.Text;

namespace SharpTest.libsharptest.Impl.Accessible
{

    enum WindowActivateInfoType
    {
        DEFAULT_LABEL_ENALBED = 0x00,
        DEFAULT_LABEL_ENALBED_WITHOUT_WINDOW = 0x01,
        DEFAULT_LABEL_DISABLED = 0x02,
        KEYBOARD = 0x04,
};

    internal interface IAtspiEvents
    {
        void onWindowActivated(IntPtr node, WindowActivateInfoType type);

        void onWindowDeactivated(IntPtr node);

        void onWindowCreated(IntPtr node);

        void onWindowDestroyed(IntPtr node);

        void onVisibilityChanged(IntPtr node, bool visible);

        void onObjectDefunct(IntPtr node);
    }


    internal class AtspiAccessibleWatcher : AccessibleWatcher, IAtspiEvents
    {
        private IntPtr mdBusProxy = IntPtr.Zero;
        private IntPtr mListener = IntPtr.Zero;
        Interop.Atspi.EventListener.AtspiCallback callback = null;

        void atspiEventsCallback(ref Interop.Atspi.EventListener.AtspiEvent ev, IntPtr data)
        {
            IntPtr error;
            String name = "", parentName = "";

            name = Interop.Atspi.Accessible.atspi_accessible_get_name(ev.source, out error);
            IntPtr parentHandle = Interop.Atspi.Accessible.atspi_accessible_get_parent(ev.source, out error);

            if (parentHandle != IntPtr.Zero) {
                parentName = Interop.Atspi.Accessible.atspi_accessible_get_name(parentHandle, out error);
                Interop.Gobject.g_object_unref(parentHandle);
            }

            if (ev.type.CompareTo("window:activate") == 0)
            {
                this.onWindowActivated(ev.source, (WindowActivateInfoType)ev.detail1);
            }
            else if (ev.type.CompareTo("window:deactivate") == 0)
            {
                this.onWindowDeactivated(ev.source);
            }
            else if (ev.type.CompareTo("window:create") == 0)
            {
                this.onWindowCreated(ev.source);
            }
            else if (ev.type.CompareTo("window:destroy") == 0)
            {
                this.onWindowDestroyed(ev.source);
            }
            else if (ev.type.CompareTo("object:state-changed:visible") == 0)
            {
                this.onVisibilityChanged(ev.source, ev.detail1 != 0);
            }
            else if (ev.type.CompareTo("object:state-changed:defunct") == 0)
            {
                this.onObjectDefunct(ev.source);
            }
        }

        public AtspiAccessibleWatcher()
        {
            Tizen.Log.Error("AURUM", "Atspi Watcher contructed");

            IntPtr enabled_variant, result, error;
            Interop.Atspi.Misc.atspi_set_main_context(Interop.Glib.g_main_context_default());
            Interop.Atspi.Misc.atspi_init();
           
            callback = new Interop.Atspi.EventListener.AtspiCallback(atspiEventsCallback);
            mListener = Interop.Atspi.EventListener.atspi_event_listener_new(callback, IntPtr.Zero, null);

            Interop.Atspi.EventListener.atspi_event_listener_register(mListener, "window:", IntPtr.Zero);
            Interop.Atspi.EventListener.atspi_event_listener_register(mListener, "object:", IntPtr.Zero);

            mdBusProxy = Interop.Gio.g_dbus_proxy_new_for_bus_sync(2, 0, IntPtr.Zero, "org.a11y.Bus", "/org/a11y/bus", "org.freedesktop.DBus.Properties", IntPtr.Zero, out error);
            enabled_variant = Interop.Gio.g_variant_new_boolean(1);
            IntPtr variant = Interop.Gio.g_variant_new("(ssv)", "org.a11y.Status", "IsEnabled", enabled_variant);
            result = Interop.Gio.g_dbus_proxy_call_sync(mdBusProxy, "Set", variant, 0, -1, IntPtr.Zero, out error);

            Interop.Gio.g_variant_unref(enabled_variant);
            Interop.Gio.g_variant_unref(result);

            System.Threading.Thread.Sleep(330);
        }

        ~AtspiAccessibleWatcher()
        {
            IntPtr enabled_variant, result, error;

            enabled_variant = Interop.Gio.g_variant_new_boolean(0);
            result = Interop.Gio.g_dbus_proxy_call_sync(mdBusProxy, "Set", 
                Interop.Gio.g_variant_new("(ssv)", "org.a11y.Status", "IsEnabled", enabled_variant), 
                0, -1, IntPtr.Zero, out error);

            Interop.Atspi.EventListener.atspi_event_listener_deregister(mListener, "window:", out error);
            Interop.Atspi.EventListener.atspi_event_listener_deregister(mListener, "object:", out error);

            Interop.Gobject.g_object_unref(mListener);
            Interop.Gobject.g_object_unref(mdBusProxy);
            Interop.Gio.g_variant_unref(enabled_variant);
            Interop.Gio.g_variant_unref(result);

            Tizen.Log.Error("AURUM", "Atspi Watcher destored");
        }


        public override AccessibleApplication getApplicationAt(int index)
        {
            IntPtr error;
            IntPtr rootHandle = Interop.Atspi.Registry.atspi_get_desktop(0);
            IntPtr childHandle = Interop.Atspi.Accessible.atspi_accessible_get_child_at_index(rootHandle, index, out error);
            Interop.Gobject.g_object_unref(rootHandle);
            return new AtspiAccessibleApplication(new AtspiAccessibleNode(childHandle));
        }

        public override int getApplicationCount()
        {
            IntPtr error;
            IntPtr rootHandle = Interop.Atspi.Registry.atspi_get_desktop(0);
            int nchild = Interop.Atspi.Accessible.atspi_accessible_get_child_count(rootHandle, out error);
            Interop.Gobject.g_object_unref(rootHandle);
            if (nchild <= 0) return 0;
            return nchild;
        }

        public override List<AccessibleApplication> getApplications()
        {
            List<AccessibleApplication> ret = new List<AccessibleApplication>();

            int nchild = getApplicationCount();
            if (nchild <= 0) return ret;

            for (int i = 0; i < nchild; i++)
            {
                var child = getApplicationAt(i);

                if (child != null && child.getAccessibleNode().isValid())
                {
                    ret.Add(child);
                }
            }

            return ret;
        }

        public void onWindowActivated(IntPtr node, WindowActivateInfoType type)
        {
            notifyAll((int)EventType.Window, (int)WindowEventType.WindowActivated, node);
        }

        public void onWindowDeactivated(IntPtr node)
        {
            notifyAll((int)EventType.Window, (int)WindowEventType.WindowActivated, node);
        }

        public void onWindowCreated(IntPtr node)
        {
            notifyAll((int)EventType.Window, (int)WindowEventType.WindowActivated, node);
        }

        public void onWindowDestroyed(IntPtr node)
        {
            notifyAll((int)EventType.Window, (int)WindowEventType.WindowActivated, node);
        }

        public void onVisibilityChanged(IntPtr node, bool visible)
        {
            notifyAll((int)EventType.Object, (int)ObjectEventType.ObjectStateVisible, node);
        }

        public void onObjectDefunct(IntPtr node)
        {
            notifyAll((int)EventType.Object, (int)ObjectEventType.ObjectStateDefunct, node);
        }
    }
}
