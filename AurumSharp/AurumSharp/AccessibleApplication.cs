using System;
using System.Collections.Generic;

namespace SharpTest.libsharptest
{
    public abstract class AccessibleApplication
    {
        private AccessibleNode mNode;

        public AccessibleApplication(AccessibleNode node)
        {
            mNode = node;
        }

        public AccessibleNode getAccessibleNode()
        {
            return mNode;
        }

        public bool isActive()
        {
            var children = getActiveWindows();
            return children.Count > 0;
        }

        public abstract List<AccessibleWindow> getWindows();

        public abstract String getPackageName();

        public List<AccessibleWindow> getActiveWindows()
        {
            var children = getWindows();
            children.RemoveAll((x) => (!(x.isActive() && x.isShowing())));
            return children;
        }
    }
}