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

#ifndef _COMPARER_H_
#define _COMPARER_H_

#include "config.h"

#include "AccessibleNode.h"
#include "UiDevice.h"
#include "UiSelector.h"

#include "PartialMatch.h"

#include <list>
#include <memory>
#include <vector>

/**
 * @class Comparer
 *
 * @ingroup aurum
 *
 * @brief Class that traverses the object tree and finds an object that match the givin condition.
 */
class Comparer {
private:
    /**
     * @brief Comparer constructor with device, selector, early return flag.
     *
     * @param[in] device @UiDevice
     * @param[in] selector @UiSelctor
     * @param[in] earlyReturn flag for early return
     *
     * @since_tizen 6.5
     */
    Comparer(const std::shared_ptr<UiDevice> device, const std::shared_ptr<UiSelector> selector,
             const bool &earlyReturn);

    /**
     * @brief Comparer destructor.
     *
     * @since_tizen 6.5
     */
    ~Comparer();

public:
    /**
     * @brief find object from device.
     *        it finds focused window then start to find object as it root.
     *
     * @param[in] device @UiDevice
     * @param[in] selector @UiSelector
     * @param[in] root @AccessibleNode root object(focused window on current state)
     *
     * @return AccessibleNode if found, else nulltpr
     *
     * @since_tizen 6.5
     */
    static std::shared_ptr<AccessibleNode> findObject(const std::shared_ptr<UiDevice> device,
                                                      const std::shared_ptr<UiSelector> selector,
                                                      const std::shared_ptr<AccessibleNode> root);

    /**
     * @brief find object from device.
     *        it finds focused window then start to find object as it root.
     *        Finds all objects to the end of tree.
     *
     * @param[in] device @UiDevice
     * @param[in] selector @UiSelector
     * @param[in] root @AccessibleNode root object(focused window on current state)
     * @param[in] earlyReturn find all object or not (default = false)
     *
     * @return AccessibleNode if found, else nulltpr
     *
     * @since_tizen 6.5
     */
    static std::vector<std::shared_ptr<AccessibleNode>> findObjects(
        const std::shared_ptr<UiDevice> device, const std::shared_ptr<UiSelector> selector,
        const std::shared_ptr<AccessibleNode> root, bool earlyReturn = false);

private:
    /**
     * @internal
     *
     * @brief Starts find object from root.
     *
     * @param[in] root @AccessibleNode
     *
     * @since_tizen 6.5
     */
    std::vector<std::shared_ptr<AccessibleNode>> findObjects(const std::shared_ptr<AccessibleNode> root);

    /**
     * @internal
     *
     * @brief It updates all partialMatches and traverse tree till given depth to find objects
     *
     * @param[in] root @AccessibleNode
     * @param[in] index node index
     * @param[in] depth tree depth
     * @param[in] partialMatches @PartialMatch list
     *
     * @since_tizen 6.5
     */
    std::vector<std::shared_ptr<AccessibleNode>> findObjects(
        const std::shared_ptr<AccessibleNode> root, const int &index, const int &depth,
        std::list<std::shared_ptr<PartialMatch>> &partialMatches);

private:
    const std::shared_ptr<UiDevice> mDevice;
    const std::shared_ptr<UiSelector> mSelector;
    bool mEarlyReturn;
};

#endif
