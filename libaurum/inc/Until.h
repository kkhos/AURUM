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

#ifndef _UTIL_H_
#define _UTIL_H_

#include <functional>
#include "ISearchable.h"
#include "UiSelector.h"
/**
 * @brief Until class
 * @since_tizen 5.5
 */
class Until {
private:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    Until();

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    Until(const std::shared_ptr<UiSelector> selector);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    Until(const Until &src);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    Until(const Until &&src);

public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    ~Until();

public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    static std::function<bool(const ISearchable *)> hasObject(
        const std::shared_ptr<UiSelector> selector);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    static std::function<std::shared_ptr<UiObject>(const ISearchable *)>
                                                 findObject(const std::shared_ptr<UiSelector> selector);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    static std::function<bool(const UiObject *)> checkable(
        const bool isCheckable);
};

#endif
