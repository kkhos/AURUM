#pragma once

/**
 * @brief Size2D Class.
 *        This class for represent specific obejct's size as width and height.
 *
 * @since_tizen 6.5
 */
template <typename T>
class Size2D {
public:
    /**
     * @brief Size2D contructor with value.
     *
     * @since_tizen 6.5
     */
    Size2D() : width{0}, height{0} {}

    /**
     * @brief Size2D contructor with source.
     *
     * @since_tizen 6.5
     */
    Size2D(const Size2D &src)
    {
        width = src.width;
        height = src.height;
    }

    /**
     * @brief Size2D contructor with type.
     *
     * @since_tizen 6.5
     */
    Size2D(const T &width, const T &height)
    {
        this->width = width;
        this->height = height;
    }

    /**
     * @brief Size2D operator for "==".
     *
     * @since_tizen 6.5
     */
    inline bool operator==(const Size2D<T>& rhs)
    {
        return this->width == rhs.width && this->height == rhs.height;
    }

    /**
     * @brief Size2D operator for "!=".
     *
     * @since_tizen 6.5
     */
    inline bool operator!=(const Size2D<T>& rhs)
    {
        return !(*this == rhs);
    }

    T width;
    T height;
};
