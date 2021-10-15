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

#ifndef _PARTIAL_MATCH_H_
#define _PARTIAL_MATCH_H_

#include <list>
#include <memory>
#include <vector>

#include "AccessibleNode.h"
#include "UiSelector.h"
/**
 * @brief PartialMatch class
 * @since_tizen 5.5
 */
class PartialMatch {
private:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    PartialMatch();

public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    PartialMatch(const std::shared_ptr<UiSelector> selector, const int absDepth);

public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    void update(const std::shared_ptr<AccessibleNode> node, int index, int depth,
                std::list<std::shared_ptr<PartialMatch>> &partialMatches);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool finalizeMatch();

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::string debugPrint();

public:
    static std::shared_ptr<PartialMatch> accept(const std::shared_ptr<AccessibleNode> node,
                                                const std::shared_ptr<UiSelector> selector,
                                                int index, int depth);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    static std::shared_ptr<PartialMatch> accept(const std::shared_ptr<AccessibleNode> node,
                                                const std::shared_ptr<UiSelector> selector,
                                                int index, int absoluteDepth,
                                                int relativeDepth);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
private:
    static bool checkCriteria(const std::shared_ptr<UiSelector> selector,
                              const std::shared_ptr<AccessibleNode> node);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */

    static bool checkCriteria(const std::string *textA, const std::string textB);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    static bool checkCriteria(const std::string *textA, const std::string textB, const bool *match);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    static bool checkCriteria(const bool *boolA, const bool boolB);

private:
    /**
     * @brief TBD
     */
    const std::shared_ptr<UiSelector>        mSelector;

    /**
     * @brief TBD
     */
    const int                                mDepth;

    /**
     * @brief TBD
     */
    std::list<std::shared_ptr<PartialMatch>> mPartialMatches;
};

#endif
