using System;
using System.Collections.Generic;

namespace SharpTest.libsharptest
{
    public abstract class AccessibleWatcher : IEventSource
    {
        private static Impl.Accessible.AtspiAccessibleWatcher instance = null;
        public static AccessibleWatcher Instance()
        {
            if (instance == null) instance = new Impl.Accessible.AtspiAccessibleWatcher();

            return instance;
        }

        internal List<AccessibleApplication> getActiveApplications()
        {
            List<AccessibleApplication> ret = new List<AccessibleApplication>();
            var apps = this.getApplications();
            apps.RemoveAll((x) => (x.isActive() == false));
            return apps;
        }

        static AccessibleWatcher Instance(AccessibleWatcher watcherImpl = null)
        {
            throw new NotImplementedException();
        }

        public abstract int getApplicationCount();

        public abstract AccessibleApplication getApplicationAt(int index);

        public abstract List<AccessibleApplication> getApplications();

        
        private Object mLock = new Object();
        public void notifyAll(int type, int type2, IntPtr src)
        {
            lock (mLock)
            {
                foreach (var consumer in mSources)
                {
                    consumer.notify(type, type2, src);
                }
            }
        }
        public void attach(IEventConsumer source)
        {
            lock (mLock)
            {
                if (source != null) mSources.Add(source);
            }
        }

        public void detach(IEventConsumer source)
        {
            lock (mLock)
            {
                if (source != null && mSources.Contains(source)) mSources.Remove(source);
            }
        }

        private HashSet<IEventConsumer> mSources = new HashSet<IEventConsumer>();

        public AccessibleWatcher()
        {
        }
    }
}