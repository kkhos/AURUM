using System;
using System.Collections.Generic;
using System.Text;

namespace SharpTest.libsharptest
{
    public class UiSelector
    {
        internal UiSelector mParent;
        internal int? mMaxDepth;
        internal int? mMinDepth;
        internal List<UiSelector> mChild = new List<UiSelector>();

        internal String mText = null;
        internal bool mMatchText = true;

        internal String mId = null;
        internal bool mMatchId = true;

        internal String mAutomationId = null;
        internal bool mMatchAutomationId = true;

        internal String mType = null;
        internal bool mMatchType = true;

        internal String mStyle = null;
        internal bool mMatchStyle = true;

        internal String mPkg = null;
        internal bool mMatchPkg = true;

        internal String mRole = null;
        internal bool mMatchRole = true;

        internal bool? mIschecked ;
        internal bool? mIscheckable ;
        internal bool? mIsclickable ;
        internal bool? mIsenabled ;
        internal bool? mIsfocused ;
        internal bool? mIsfocusable;
        internal bool? mIsscrollable ;
        internal bool? mIsselected ;
        internal bool? mIsshowing ;
        internal bool? mIsactive ;
        internal bool? mIsvisible ;
        internal bool? mIsselectable ;
    }
}

