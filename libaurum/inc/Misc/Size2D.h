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
