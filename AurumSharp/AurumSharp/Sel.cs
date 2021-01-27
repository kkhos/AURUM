using System;

namespace SharpTest.libsharptest
{
    public class Sel
    {
        public  static UiSelector depth(int v)
        {
            var sel = new UiSelector();
            sel.mMaxDepth = 1;
            sel.mMinDepth = 1;
            return sel;
        }

        public static UiSelector text(string text)
        {
            var sel = new UiSelector();
            sel.mText = text;
            return sel;
        }


    }
}