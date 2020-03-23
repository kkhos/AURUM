#include "PartialMatch.h"

#include <iostream>
#include <set>

#include "loguru.hpp"

bool PartialMatch::checkCriteria(const std::shared_ptr<UiSelector> selector,
                                 const AccessibleNode *node)
{
    if (selector->mPkg.length() > 0 && selector->mPkg.compare(node->getPkg()))
        return false;
    if (selector->mRes.length() > 0 && selector->mRes.compare(node->getRes()))
        return false;
    if (selector->mText.length() > 0 && selector->mText.compare(node->getText()))
        return false;
    if (selector->mDesc.length() > 0 && selector->mDesc.compare(node->getDesc()))
        return false;
    if (selector->mType.length() > 0 && selector->mType.compare(node->getType()))
        return false;

    LOG_F(INFO, "node mPkg :%s, selector->desc :%s | %ld", node->getPkg().c_str(),
          selector->mPkg.c_str(), selector->mPkg.length());
    LOG_F(INFO, "node mRes :%s, selector->desc :%s | %ld", node->getRes().c_str(),
          selector->mRes.c_str(), selector->mRes.length());
    LOG_F(INFO, "node mText :%s, selector->desc :%s | %ld", node->getText().c_str(),
          selector->mText.c_str(), selector->mText.length());
    LOG_F(INFO, "node mDesc :%s, selector->desc :%s | %ld", node->getDesc().c_str(),
          selector->mDesc.c_str(), selector->mDesc.length());
    LOG_F(INFO, "node mType :%s, selector->type :%s | %ld", node->getType().c_str(),
          selector->mType.c_str(), selector->mType.length());

    return true;
}

PartialMatch::PartialMatch() : mSelector{nullptr}, mDepth{-1}, mPartialMatches{}
{
}

PartialMatch::PartialMatch(const std::shared_ptr<UiSelector> selector, const int absDepth)
    : mSelector{selector}, mDepth{absDepth}, mPartialMatches{}
{
}

std::shared_ptr<PartialMatch> PartialMatch::accept(const AccessibleNode *node,
                                                   const std::shared_ptr<UiSelector> selector,
                                                   int index, int depth)
{
    return PartialMatch::accept(node, selector, index, depth, depth);
}

std::shared_ptr<PartialMatch> PartialMatch::accept(const AccessibleNode *node,
                                                   const std::shared_ptr<UiSelector> selector,
                                                   int index, int absoluteDepth,
                                                   int relativeDepth)
{
    PartialMatch *match = nullptr;

    if (PartialMatch::checkCriteria(selector, node)) {
        LOG_SCOPE_F(INFO, "New Match found %p %d", selector, absoluteDepth);
        match = new PartialMatch(selector, absoluteDepth);
    }

    return std::shared_ptr<PartialMatch>(match);
}

void PartialMatch::update(
    const AccessibleNode *node, int index, int depth,
    std::list<std::shared_ptr<PartialMatch>> &partialMatches)
{
    for (auto childSelector : mSelector->mChild) {
        auto match = PartialMatch::accept(node, childSelector, index, depth,
                                          depth - mDepth);
        if (match) {
            mPartialMatches.push_back(match);
            partialMatches.push_front(match);
        }
    }
}

bool PartialMatch::finalizeMatch()
{
    std::set<std::shared_ptr<UiSelector>> matches;
    for (auto match : mPartialMatches) {
        if (match->finalizeMatch()) {
            matches.insert(match->mSelector);
        }
    }

    for (auto sel : mSelector->mChild) {
        if (!matches.count(sel)) return false;
    }
    return true;
}
