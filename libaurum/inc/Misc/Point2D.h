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

namespace Aurum {

/**
 * @class Point2D
 *
 * @ingroup aurum
 *
 * @brief Class for X, Y coordinate expression and operation.
 */
template <typename T>
class Point2D {
public:
    /**
     * @brief Point2D constructor.
     *
     * @since_tizen 5.5
     */
    Point2D() : x{0}, y{0} {}

    /**
     * @brief Point2D constructor with Point2D source.
     *
     * @param[in] src Point2D source
     *
     * @since_tizen 5.5
     */
    Point2D(const Point2D &src)
    {
        x = src.x;
        y = src.y;
    }

    /**
     * @brief Point2D constructor with template.
     *
     * @param[in] x x coordinate
     * @param[in] y y coordinate
     *
     * @since_tizen 5.5
     */
    Point2D(const T &x, const T &y)
    {
        this->x = x;
        this->y = y;
    }

    /**
     * @brief Checks given coordinate is same as this or not.
     *
     * @param[in] rhs @Point2D
     *
     * @return true if same, else false
     *
     * @since_tizen 5.5
     */
    inline bool operator==(const Point2D<T>& rhs)
    {
        return this->x == rhs.x && this->y == rhs.y;
    }

    /**
     * @brief Checks given coordinate is not same as this or not.
     *
     * @param[in] rhs @Point2D
     *
     * @return true if same, else false
     *
     * @since_tizen 5.5
     */
    inline bool operator!=(const Point2D<T>& rhs)
    {
        return !(*this == rhs);
    }

    T x;
    T y;
};

}

#endif
