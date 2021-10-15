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

#ifndef _WAITER_H_
#define _WAITER_H_

#include "ISearchable.h"

#include <functional>
/**
 * @brief Waiter class
 * @since_tizen 5.5
 */
class Waiter {
private:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    Waiter();

public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    Waiter(const ISearchable *searchableObject,
           const UiObject *uiObject = nullptr);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    ~Waiter();

public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    template <typename R>
    R waitFor(const std::function<R(const ISearchable *)> condition) const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    template <typename R>
    R waitFor(const std::function<R(const UiObject *)> object) const;

private:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    const ISearchable *mSearchableObject;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    const UiObject *mUiObject;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    const int          WAIT_INTERVAL_MS;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    const int          WAIT_TIMEOUT_MS;
};

#endif
