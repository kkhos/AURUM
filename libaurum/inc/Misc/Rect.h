#pragma once

#include "Point2D.h"

/**
 * @brief Rect Class
 * @since_tizen 5.5
 */
template <typename T>
class Rect {
public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    Rect() : mTopLeft{0, 0}, mBottomRight{0, 0} {}

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    Rect(const Point2D<T> &tl, const Point2D<T> &br)
        : mTopLeft(tl), mBottomRight(br)
    {
    }

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    Rect(const T &x1, const T &y1, const T &x2, const T &y2)
        : mTopLeft{x1, y1}, mBottomRight{x2, y2}
    {
    }

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    Rect(const Rect<T> &src)

    {
        this->mTopLeft = Point2D<int>{src.mTopLeft};
        this->mBottomRight = Point2D<int>{src.mBottomRight};
    }

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    Point2D<T> midPoint() const
    {
        return Point2D<T>{mTopLeft.x + static_cast<T>(width() / 2),
                          mTopLeft.y + static_cast<T>(height() / 2)};
    }

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    T width() const { return mBottomRight.x - mTopLeft.x; }

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    T height() const { return mBottomRight.y - mTopLeft.y; }

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    inline bool operator==(const Rect<T>& rhs)
    {
        return this->mTopLeft == rhs.mTopLeft && this->mBottomRight == rhs.mBottomRight;
    }

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    inline bool operator!=(const Rect<T>& rhs){
        return !(*this == rhs);
    }

    /**
     * @brief TBD
     */
    Point2D<T> mTopLeft;

    /**
     * @brief TBD
     */
    Point2D<T> mBottomRight;
};
