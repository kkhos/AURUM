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
 * @class PartialMatch
 *
 * @ingroup aurum
 *
 * @brief Class that defines a methods to perform multiple condition of search object.
 *        It works like helper class for @Comparer.
 */
class PartialMatch {
private:
    /**
     * @brief PartialMatch constructor.
     *
     * @since_tizen 5.5
     */
    PartialMatch();

public:
    /**
     * @brief PartialMatch constructor with selector, absolute depth.
     *
     * @since_tizen 5.5
     */
    PartialMatch(const std::shared_ptr<UiSelector> selector, const int absDepth);

public:
    /**
     * @brief Updates all PartialMatches class.
     *
     * @param node target @AssessibleNode
     * @param index node index
     * @param depth next depth
     * @param partialMatches @PartialMatch list
     *
     * @since_tizen 5.5
     */
    void update(const std::shared_ptr<AccessibleNode> node, int index, int depth,
                std::list<std::shared_ptr<PartialMatch>> &partialMatches);

    /**
     * @brief Saves match result if found.
     *
     * @return true if matched child exist, else false
     *
     * @since_tizen 5.5
     */
    bool finalizeMatch();

    /**
     * @brief Prints selector information.
     *
     * @since_tizen 5.5
     */
    std::string debugPrint();

public:

    /**
     * @brief Checks search criteria on given depth objects.
     *
     * @param node target @AccessibleNode
     * @param selector @UiSelector
     * @param index node index
     * @param depth searching depth from ui layout hierarchy
     *
     * @return @PartialMatch
     * @since_tizen 5.5
     */
    static std::shared_ptr<PartialMatch> accept(const std::shared_ptr<AccessibleNode> node,
                                                const std::shared_ptr<UiSelector> selector,
                                                int index, int depth);

    /**
     * @brief Checks search criteria on given depth objects.
     *
     * @param node target @AccessibleNode
     * @param selector @UiSelector
     * @param index node index
     * @param absoluteDepth absolute depth
     * @param relativeDepth relative depth
     *
     * @return @PartialMatch
     *
     * @since_tizen 5.5
     */
    static std::shared_ptr<PartialMatch> accept(const std::shared_ptr<AccessibleNode> node,
                                                const std::shared_ptr<UiSelector> selector,
                                                int index, int absoluteDepth,
                                                int relativeDepth);

private:

    /**
     * @brief Checks given node's search criteria are satisfied or not.
     *
     * @param selector @UiSelector
     * @param node @AccessibleNode
     *
     * @return ture if satisfied, else false
     *
     * @since_tizen 5.5
     */
    static bool checkCriteria(const std::shared_ptr<UiSelector> selector,
                              const std::shared_ptr<AccessibleNode> node);

    /**
     * @brief Checks text matched or not.
     *
     * @param textA string
     * @param textB string
     *
     * @return ture if matched, else false
     *
     * @since_tizen 5.5
     */
    static bool checkCriteria(const std::string *textA, const std::string textB);

    /**
     * @brief Checks boolean value matched or not.
     *
     * @param boolA bool
     * @param boolA bool
     *
     * @return ture if matched, else false
     *
     * @since_tizen 5.5
     */
    static bool checkCriteria(const bool *boolA, const bool boolB);

private:
    const std::shared_ptr<UiSelector>        mSelector;
    const int                                mDepth;
    std::list<std::shared_ptr<PartialMatch>> mPartialMatches;
};

#endif
