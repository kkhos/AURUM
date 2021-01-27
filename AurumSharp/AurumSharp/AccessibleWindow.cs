using System;

namespace SharpTest.libsharptest
{
    public class AccessibleWindow
    {
        private AccessibleApplication mApp = null;

        private AccessibleNode mNode = null;

        public AccessibleWindow(AccessibleApplication app, AccessibleNode node)
        {
            mApp = app;
            mNode = node;
        }

        internal String getTitle()
        {
            return mNode.getText();
        }

        internal bool isShowing()
        {
            return mNode.isShowing();
        }

        internal bool isActive()
        {
            return mNode.isActive();
        }

        internal AccessibleApplication getApplication()
        {
            return mApp;
        }

        internal AccessibleNode getAccessibleNode()
        {
            return mNode;
        }



    }
}