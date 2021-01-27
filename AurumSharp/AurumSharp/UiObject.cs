using System;
using System.Collections.Generic;
using System.Text;

namespace SharpTest.libsharptest
{
    public class Node
    {
        public Node(UiObject uiObject, List<Node> nodeChildren)
        {
        }
    }

    public class UiObject : ISearchable
    {

        private UiDevice mDevice;

        private UiSelector mSelector;

        private AccessibleNode mNode;

        // private const Waiter* mWaiter;

        private const int LOGNCLICK_INTERVAL = 300;

        public UiObject(UiDevice uiDevice, UiSelector selector, AccessibleNode found)
        {
            this.mDevice = uiDevice;
            this.mSelector = selector;
            this.mNode = found;
        }

        public UiObject FindObject(UiSelector selector)
        {
            var node = Comparer.findObject(mDevice, selector, getAccessibleNode());
            if (node != null) return new UiObject(mDevice, selector, node);
            return null;
        }

        public List<UiObject> FindObjects(UiSelector selector)
        {
            List<UiObject> result = new List<UiObject>();
            var nodes = Comparer.findObjects(mDevice, selector, getAccessibleNode());
            foreach (var node in nodes)
            {
                if (node != null)
                {
                    result.Add(new UiObject(mDevice, selector, node));
                }
            }
            return result;
        }

        public bool HasObject(UiSelector selector)
        {
            var node = Comparer.findObject(this.mDevice, selector, getAccessibleNode());
            if (node != null) return true;
            return false;
        }

        public UiSelector getSelector()
        {
            return this.mSelector;
        }

        public UiObject getParent()
        {
            var parent = getAccessibleNode().getParent();
            if (parent == null || (parent != null && !parent.isValid())) return null;
            return new UiObject(mDevice, mSelector, parent);
        }

        public int getChildCount()
        {
            return getAccessibleNode().getChildCount();
        }

        public List<UiObject> getChildren()
        {
            var sel = Sel.depth(1);
            return this.FindObjects(sel);
        }

        public UiObject getChildAt(int index)
        {
            var childNode = getAccessibleNode().getChildAt(index);
            if (childNode != null) return new UiObject(mDevice, mSelector, childNode);
            return null;
        }

        public Node getDescendant()
        {
            List<Node> nodeChildren = new List<Node>();
            var children = getChildren();

            foreach (var child in children)
            {
                nodeChildren.Add(child.getDescendant());
            }
            return new Node(this, nodeChildren);
        }

        public String getApplicationPackage()
        {
            return getAccessibleNode().getPkg();
        }

        public String getId()
        {
            return getAccessibleNode().getId();
        }

        public String getAutomationId()
        {
            return getAccessibleNode().getAutomationId();
        }

        public String getElementType()
        {
            return getAccessibleNode().getType();
        }

        public String getElementStyle()
        {
            return getAccessibleNode().getStyle();
        }

        public String getText()
        {
            return getAccessibleNode().getText();
        }

        public String getRole()
        {
            return getAccessibleNode().getRole();
        }

        public void setText(String text)
        {
            getAccessibleNode().setText(text);
        }

        public Rect getBoundingBox()
        {
            return getAccessibleNode().getBoundingBox();
        }

        public bool isCheckable()
        {
            return getAccessibleNode().isCheckable();
        }

        public bool isChecked()
        {
            return getAccessibleNode().isChecked();
        }

        public bool isClickable()
        {
            return getAccessibleNode().isClickable();
        }

        public bool isEnabled()
        {
            return getAccessibleNode().isEnabled();
        }

        public bool isFocusable()
        {
            return getAccessibleNode().isFocusable();
        }

        public bool isFocused()
        {
            return getAccessibleNode().isFocused();
        }

        public bool isLongClickable()
        {
            return getAccessibleNode().isLongClickable();
        }

        public bool isScrollable()
        {
            return getAccessibleNode().isScrollable();
        }

        public bool isSelectable()
        {
            return getAccessibleNode().isSelectable();
        }

        public bool isSelected()
        {
            return getAccessibleNode().isSelected();
        }

        public bool isVisible()
        {
            return getAccessibleNode().isVisible();
        }

        public bool isShowing()
        {
            return getAccessibleNode().isShowing();
        }

        public bool isActive()
        {
            return getAccessibleNode().isActive();
        }

        public void click()
        {
            mNode.refresh();
            Rect rect = mNode.getBoundingBox();
            Point2D midPoint = rect.midPoint();
            mDevice.Click(midPoint.x, midPoint.y);
        }

        public void longClick( int intv = LOGNCLICK_INTERVAL)
        {
            mNode.refresh();
            Rect rect = mNode.getBoundingBox();
            Point2D midPoint = rect.midPoint();
            mDevice.Click(midPoint.x, midPoint.y, intv);
        }

        public bool DoAtspiActivate()
        {
            return getAccessibleNode().doAction("activate");
        }

        public void refresh()
        {
            getAccessibleNode().refresh();
        }

        public bool isValid()
        {
            return getAccessibleNode().isValid();
        }

        public AccessibleNode getAccessibleNode()
        {
            return this.mNode;
        }
    }
}
