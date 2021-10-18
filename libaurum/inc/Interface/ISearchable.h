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
     * @since_tizen 5.5
     */
    virtual ~ISearchable() {}

    /**
     * @brief Checks that there is an object that are satisfied with selector condition.
     *
     * @param[in] selector @UiSelctor
     *
     * @return true if object has, else false
     *
     * @since_tizen 5.5
     */
    virtual bool hasObject(const std::shared_ptr<UiSelector> selector) const = 0;

    /**
     * @brief Finds object that is satisfied with selector condition.
     *
     * @param[in] selector @UiSelctor
     *
     * @return UiObject if succeed, else nulltpr
     *
     * @since_tizen 5.5
     */
    virtual std::shared_ptr<UiObject> findObject(
        const std::shared_ptr<UiSelector> selector) const = 0;

    /**
     * @brief Finds objects that are satisfied with selector condition.
     *
     * @param[in] selector @UiSelctor
     *
     * @return UiObject vector if succeed, else nulltpr
     *
     * @since_tizen 5.5
     */
    virtual std::vector<std::shared_ptr<UiObject>> findObjects(
        const std::shared_ptr<UiSelector> selector) const = 0;
};

}

#endif
