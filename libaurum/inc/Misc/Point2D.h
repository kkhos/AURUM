#pragma once

/**
 * @brief Point2d Class
 * @since_tizen 5.5
 */
template <typename T>
class Point2D {
public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    Point2D() : x{0}, y{0} {}

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    Point2D(const Point2D &src)
    {
        x = src.x;
        y = src.y;
    }

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    Point2D(const T &x, const T &y)
    {
        this->x = x;
        this->y = y;
    }

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    inline bool operator==(const Point2D<T>& rhs)
    {
        return this->x == rhs.x && this->y == rhs.y;
    }

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    inline bool operator!=(const Point2D<T>& rhs)
    {
        return !(*this == rhs);
    }

    /**
     * @brief TBD
     */
    T x;

    /**
     * @brief TBD
     */
    T y;
};
