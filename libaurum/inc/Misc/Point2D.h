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

#ifndef _POINT2D_H_
#define _POINT2D_H_

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

#endif
