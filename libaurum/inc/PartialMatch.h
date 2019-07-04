#ifndef PARTIAL_MATCH_H
#define PARTIAL_MATCH_H

#include <list>
#include <memory>
#include <vector>

#include "AccessibleNode.h"
#include "UiSelector.h"

class PartialMatch {
private:
    PartialMatch();

    const UiSelector *                       mSelector;
    const int                                mDepth;
    std::list<std::shared_ptr<PartialMatch>> mPartialMatches;

public:
    PartialMatch(const UiSelector *selector, const int absDepth);
    void update(const AccessibleNode *node, int index, int depth,
                std::list<std::shared_ptr<PartialMatch>> &partialMatches);
    bool finalizeMatch();

public:
    static std::shared_ptr<PartialMatch> accept(const AccessibleNode *node,
                                                const UiSelector *    selector,
                                                int index, int depth);
    static std::shared_ptr<PartialMatch> accept(const AccessibleNode *node,
                                                const UiSelector *    selector,
                                                int index, int absoluteDepth,
                                                int relativeDepth);

private:
    static bool checkCriteria(const UiSelector *    sel,
                              const AccessibleNode *node);
};

#endif