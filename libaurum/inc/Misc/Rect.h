/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *               http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#ifndef _RECT_H_
#define _RECT_H_

#include "Point2D.h"

namespace Aurum {

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

}

#endif
