using System;
using System.Collections.Generic;
using System.Text;

namespace SharpTest.libsharptest
{
    enum AccessibleNodeInterface
    {
        ACTION          = 0x0001,
        COLLECTION      = 0X0002,
        COMPONENT       = 0X0004,
        DOCUMENT        = 0X0008,

        EDITABLETEXT    = 0X0010,
        HYPERTEXT       = 0X0020,
        IMAGE           = 0X0040,
        SELECTION       = 0X0080,

        TEXT            = 0X0100,
        VALUE           = 0X0200,
        ACCESSIBLE      = 0X0400,
        TABLE           = 0X0800,

        TABLECELL       = 0X1000,
    };

    enum NodeFeatureProperties
    {
        CHECKABLE       = 0x0001,
        CHECKED         = 0X0002,
        CLICKABLE       = 0X0004,
        ENABLED         = 0X0008,

        FOCUSABLE       = 0X0010,
        FOCUSED         = 0X0020,
        LONGCLICKABLE   = 0X0040,
        SCROLLABLE      = 0X0080,

        SELECTABLE      = 0X0100,
        SELECTED        = 0X0200,
        VISIBLE         = 0X0400,
        SHOWING         = 0X0800,
        ACTIVE          = 0X1000,
        INVALID         = 0X2000,
    };

    public abstract class AccessibleNode : IEventConsumer
    {

        private Object mLock = new object();

        private bool mValid = true;

        protected string mText = "";

        protected string mPkg = "";

        protected string mRole = "";

        protected string mId = "";

        protected string mAutomationId = "";

        protected string mType = "";

        protected string mStyle = "";

        protected Rect mBoundingBox = new Rect(0, 0, 0, 0);

        private int mSupportingIfaces = 0;

        private int mFeatureProperty = 0;

        public override string ToString()
        {
            return description();
        }

        public string description()
        {
            StringBuilder sb = new StringBuilder();

            sb.AppendFormat("{{");
            sb.AppendFormat("\"mId\":\"{0}\", ", mId);
            sb.AppendFormat("\"mAutomationId\":\"{0}\", ", mAutomationId);
            sb.AppendFormat("\"mRole\":\"{0}\", ",mRole);
            sb.AppendFormat("\"mText\":\"{0}\", ",mText);
            sb.AppendFormat("\"mPkg\":\"{0}\", ",mPkg); 
            sb.AppendFormat("\"mType\":\"{0}\", ",mType); 
            sb.AppendFormat("\"mStyle\":\"{0}\", ",mStyle);
            sb.AppendFormat("}}");

            return sb.ToString();
        }

        public abstract int getChildCount();
        public abstract AccessibleNode getChildAt(int index);
        public abstract List<AccessibleNode> getChildren();
        public abstract AccessibleNode getParent();

        public void invalidate()
        {
            lock(mLock)
            {
                mValid = false;
            }
        }

        internal bool isValid()
        {
            lock (mLock)
            {
                if (getRawhandler() == IntPtr.Zero || !mValid)
                {
                    Tizen.Log.Info("AURUM", string.Format("Node({0:X8}) is invalid Ptr{1:X8} mValid:{2}", this, getRawhandler().ToInt32(), mValid.ToString()));
                    return false;
                }
            }
            return true;
        }

        public void notify(int type, int type2, IntPtr src)
        {
            IntPtr nodeHandler = getRawhandler();
            if ((EventType)type == EventType.Object && (ObjectEventType)type2 == ObjectEventType.ObjectStateDefunct)
            {
                if (nodeHandler == src) invalidate();
            }
        }

        internal string getPkg()
        {
            return mPkg;
        }

        internal string getId()
        {
            return mId;
        }

        internal string getAutomationId()
        {
            return mAutomationId;
        }

          internal string getText()
        {
            return mText;
        }

        internal string getRole()
        {
            return mRole;
        }

        internal Rect getBoundingBox()
        {
            return mBoundingBox;
        }

        internal bool isCheckable()
        {
            return hasFeatureProperty(NodeFeatureProperties.CHECKABLE);
        }

        internal bool isChecked()
        {
            return hasFeatureProperty(NodeFeatureProperties.CHECKED);
        }

        internal bool isClickable()
        {
            return hasFeatureProperty(NodeFeatureProperties.CLICKABLE);
        }

        internal bool isEnabled()
        {
            return hasFeatureProperty(NodeFeatureProperties.ENABLED);
        }

        internal bool isFocusable()
        {
            return hasFeatureProperty(NodeFeatureProperties.FOCUSABLE);
        }

        internal bool isFocused()
        {
            return hasFeatureProperty(NodeFeatureProperties.FOCUSED);
        }

        internal bool isLongClickable()
        {
            return hasFeatureProperty(NodeFeatureProperties.LONGCLICKABLE);
        }

        internal bool isScrollable()
        {
            return hasFeatureProperty(NodeFeatureProperties.SCROLLABLE);
        }

        internal bool isSelectable()
        {
            return hasFeatureProperty(NodeFeatureProperties.SELECTABLE);
        }

        internal bool isSelected()
        {
            return hasFeatureProperty(NodeFeatureProperties.SELECTED);
        }

        internal bool isVisible()
        {
            return hasFeatureProperty(NodeFeatureProperties.VISIBLE);
        }

        internal bool isShowing()
        {
            return hasFeatureProperty(NodeFeatureProperties.SHOWING);
        }

        internal bool isActive()
        {
            return hasFeatureProperty(NodeFeatureProperties.ACTIVE);
        }

        internal String getStyle()
        {
            return mStyle;
        }

        internal String getType()
        {
            return mType;
        }

        internal  bool isSupporting(AccessibleNodeInterface thisIface)
        {
            return (mSupportingIfaces & (int)thisIface) != 0;
        }

        internal bool hasFeatureProperty(NodeFeatureProperties prop)
        {
            return (mFeatureProperty & (int)prop) != 0;
        }

        internal void setFeatureProperty(NodeFeatureProperties prop, bool has)
        {
            if (has)
                mFeatureProperty |= (int)(prop);
            else
                mFeatureProperty &= ~(int)(prop);
        }

        public abstract IntPtr getRawhandler();
        public abstract void refresh();
        public abstract bool doAction(string v);
        public abstract List<string> getActions();
        public abstract void setText(string text);
    }
}