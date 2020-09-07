#pragma once

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
    void debugPrint();

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