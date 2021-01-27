using System;

namespace SharpTest.libsharptest
{
    public class Point2D : IEquatable<Point2D>
    {
        public int x { get; internal set; }
        public int y { get; internal set; }

        public Point2D(int p1, int p2)
        {
            x = p1;
            y = p2;
        }

        public bool Equals(Point2D other)
        {
            return other.x == x && other.y == y;
        }
    }
}