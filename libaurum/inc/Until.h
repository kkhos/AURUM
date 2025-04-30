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

#ifndef _UNTIL_H_
#define _UNTIL_H_

#include <functional>
#include "ISearchable.h"
#include "UiSelector.h"

namespace Aurum {

/**
 * @class Until
 *
 * @ingroup Aurum
 *
 * @brief Class for use in a @Waiter class waiting for a particular object to find
 *        or waiting for a state change.
 */
class Until {
private:
    /**
     * @brief Until constructor.
     *
     * @since_tizen 6.5
     */
    Until();

    /**
     * @brief Until constructor with selector.
     *
     * @param[in] selector @UiSelector
     *
     * @since_tizen 6.5
     */
    Until(const std::shared_ptr<UiSelector> selector);

    /**
     * @brief UiSelector constructor with source.
     *
     * @param[in] src reference of Until source
     *
     * @since_tizen 6.5
     */
    Until(const Until &src);

    /**
     * @brief UiSelector constructor with source.
     *
     * @param[in] src Rvalue reference of Util source
     *
     * @since_tizen 6.5
     */
    Until(const Until &&src);

public:
    /**
     * @brief UiSelector destructor.
     *
     * @since_tizen 6.5
     */
    ~Until();

public:
    /**
     * @brief Checks that there is an object that are satisfied with selector condition.
     *
     * @param[in] selector @UiSelector
     *
     * @return function that performs hasObject
     *         returned function will return true if object has, else false
     *
     * @since_tizen 6.5
     */
    static std::function<bool(const ISearchable *)> hasObject(
        const std::shared_ptr<UiSelector> &selector);

    /**
     * @brief Checks that there is an object that are satisfied with selector condition.
     *
     * @param[in] selector @UiSelector
     *
     * @return function that performs findObject
     *         returned function will return obj if succeed, else nullptr
     *
     * @since_tizen 6.5
     */
    static std::function<std::shared_ptr<UiObject>(const ISearchable *)>
                                                 findObject(const std::shared_ptr<UiSelector> &selector);

    /**
     * @brief Checks the given object on checkable state.
     *
     * @param[in] isCheckable checkable or not
     *
     * @return function that check checkable
     *         returned function will return true if checkable, else false
     *
     * @since_tizen 6.5
     */
    static std::function<bool(const UiObject *)> checkable(
        const bool isCheckable);

    /**
     * @brief Checks that there are objects that are satisfied with selector condition.
     *
     * @param[in] selector @UiSelector
     *
     * @return function that performs findObjects
     *         returned function will return vector if succeed, else vector size is zero
     *
     * @since_tizen 7.0
     */
    static std::function<std::vector<std::shared_ptr<UiObject>>(const ISearchable *)>
                                                 findObjects(const std::shared_ptr<UiSelector> &selector);
};

}

#endif
