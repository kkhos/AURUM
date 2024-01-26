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

#ifndef _IMATCHES_H_
#define _IMATCHES_H_

#include "config.h"

#include "UiObject.h"
#include "UiSelector.h"

#include <memory>
#include <vector>

namespace Aurum {

/**
 * @class IMatches
 *
 * @ingroup aurum
 *
 * @brief IMatches interface that defines a methods for the behavior search object in ui layout hierarchy.
 */
class IMatches {
public:
    /**
     * @brief IMatches Destructor.
     *
     * @since_tizen 6.5
     */
    virtual ~IMatches() {}

    /**
     * @brief Get the Matches object that satisfied with the selector condition in the object tree.
     *        This method is similar to findObjects, but getMatches is more faster than findObjects.
     *
     * @param[in] selector @UiSelector
     * @param[in] earlyReturn boolean
     *
     * @return the list of found UiObject pointer vector
     *
     * @since_tizen 8.0
     */
    virtual std::vector<std::shared_ptr<UiObject>> getMatches(
        const std::shared_ptr<UiSelector> selector, const bool earlyReturn) const = 0;

    /**
     * @brief Get the object that satisfied with both condition in the object tree.
     *
     * @param[in] firstSelector @UiSelector
     * @param[in] secondSelector @UiSelector
     * @param[in] earlyReturn boolean
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
