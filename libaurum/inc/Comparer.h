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

#include "config.h"

#include "AccessibleNode.h"
#include "UiDevice.h"
#include "UiSelector.h"

#include "PartialMatch.h"

#include <list>
#include <memory>
#include <vector>

/**
 * @brief Comparer class
 * @since_tizen 5.5
 */
class Comparer {
private:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    Comparer(const std::shared_ptr<UiDevice> device, const std::shared_ptr<UiSelector> selector,
             const bool &earlyReturn);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    ~Comparer();

public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    static std::shared_ptr<AccessibleNode>          findObject(const std::shared_ptr<UiDevice> device,
                                                    const std::shared_ptr<UiSelector> selector,
                                                    const std::shared_ptr<AccessibleNode> root);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    static std::vector<std::shared_ptr<AccessibleNode>> findObjects(
        const std::shared_ptr<UiDevice> device, const std::shared_ptr<UiSelector> selector,
        const std::shared_ptr<AccessibleNode> root, bool earlyReturn = false);

private:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::vector<std::shared_ptr<AccessibleNode>> findObjects(const std::shared_ptr<AccessibleNode> root);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::vector<std::shared_ptr<AccessibleNode>> findObjects(
        const std::shared_ptr<AccessibleNode> root, const int &index, const int &depth,
        std::list<std::shared_ptr<PartialMatch>> &partialMatches);

private:
    /**
     * @brief TBD
     */
    const std::shared_ptr<UiDevice> mDevice;

    /**
     * @brief TBD
     */
    const std::shared_ptr<UiSelector> mSelector;

    /**
     * @brief TBD
     */
    bool              mEarlyReturn;
};