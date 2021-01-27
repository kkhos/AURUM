using System;
using System.Collections.Generic;
using System.Text;

namespace SharpTest.libsharptest
{
    

    public class Rect : IEquatable<Rect>
    {
        private Point2D mTopLeft;
        private Point2D mBottomRight;

        public Rect(int x1, int y1, int x2, int y2)
        {
            mTopLeft = new Point2D(x1, y1);
            mBottomRight = new Point2D(x2, y2);
        }

        public Point2D midPoint()
        {
            return new Point2D(mTopLeft.x + (width() / 2), mTopLeft.y + (height() / 2));
        }

        public int width()
        {
            return mBottomRight.x - mTopLeft.x;
        }

        public int height()
        {
           return mBottomRight.y - mTopLeft.y;
        }

        public bool Equals(Rect other)
        {
            return mTopLeft == other.mTopLeft && mBottomRight == other.mBottomRight;
        }
    }
}
