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

#ifndef _ISEARCHABLE_H_
#define _ISEARCHABLE_H_

#include "config.h"

#include "UiSelector.h"

#include <memory>
#include <vector>

namespace Aurum {

class UiObject;

/**
 * @class ISearchable
 *
 * @ingroup aurum
 *
 * @brief ISearchable interface that defines a methods for the behavior search object in ui layout hierarchy.
 */
class ISearchable {
public:
    /**
     * @brief ISearchable Destructor.
     *
     * @since_tizen 6.5
     */
    virtual ~ISearchable() {}

    /**
     * @brief Checks that there is an object that are satisfied with selector condition.
     *
     * @param[in] selector @UiSelector
     *
     * @return true if object has, else false
     *
     * @since_tizen 6.5
     */
    virtual bool hasObject(const std::shared_ptr<UiSelector> selector) const = 0;

    /**
     * @brief Finds object that is satisfied with selector condition.
     *
     * @param[in] selector @UiSelector
     *
     * @return UiObject if succeed, else nullptr
     *
     * @since_tizen 6.5
     */
    virtual std::shared_ptr<UiObject> findObject(
        const std::shared_ptr<UiSelector> selector) const = 0;

    /**
     * @brief Finds objects that are satisfied with selector condition.
     *
     * @param[in] selector @UiSelector
     *
     * @return UiObject vector if succeed, else nullptr
     *
     * @since_tizen 6.5
     */
    virtual std::vector<std::shared_ptr<UiObject>> findObjects(
        const std::shared_ptr<UiSelector> selector) const = 0;

    /**
     * @brief Get the Matches object that satisfied with the selector condition in the object tree.
     *        This method is similar to findObjects, but getMatches is more faster than findObjects.
     *        A single dbus method call occurs internally and retrieves a list of objects
     *        that satisfy the selector specified in uifw.
     *
     * @param[in] selector @UiSelector
     * @param[in] earlyReturn boolean Returns immediately when a matched object is found
     *
     * @return the list of found UiObject pointer vector
     *
     * @since_tizen 8.0
     */
    virtual std::vector<std::shared_ptr<UiObject>> getMatches(
        const std::shared_ptr<UiSelector> selector, const bool earlyReturn) const = 0;

    /**
     * @brief Get the object that satisfied with both condition in the object tree.
     *        Additional object detection is performed using the second selector for the object
     *        found in the first selector.
     *
     * @param[in] firstSelector @UiSelector
     * @param[in] secondSelector @UiSelector
     * @param[in] earlyReturn boolean Returns immediately when a matched object is found
     *
     * @return the list of found UiObject pointer vector
     *
     * @since_tizen 8.0
     */
    virtual std::vector<std::shared_ptr<UiObject>> getMatchesInMatches(
        const std::shared_ptr<UiSelector> firstSelector, const std::shared_ptr<UiSelector> secondSelector, const bool earlyReturn) const = 0;
};

}

#endif
