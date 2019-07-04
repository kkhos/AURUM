#include "PartialMatch.h"

#include <iostream>
#include <set>

#include "loguru.hpp"

bool PartialMatch::checkCriteria(const UiSelector *    sel,
                                 const AccessibleNode *node)
{
    if (sel->mPkg.length() > 0 && sel->mPkg.compare(node->getPkg()))
        return false;
    if (sel->mRes.length() > 0 && sel->mRes.compare(node->getRes()))
        return false;
    if (sel->mText.length() > 0 && sel->mText.compare(node->getText()))
        return false;
    if (sel->mDesc.length() > 0 && sel->mDesc.compare(node->getDesc()))
        return false;
    if (sel->mType.length() > 0 && sel->mType.compare(node->getType()))
        return false;

    LOG_F(INFO, "node mPkg :%s, sel->desc :%s | %ld", node->getPkg().c_str(),
          sel->mPkg.c_str(), sel->mPkg.length());
    LOG_F(INFO, "node mRes :%s, sel->desc :%s | %ld", node->getRes().c_str(),
          sel->mRes.c_str(), sel->mRes.length());
    LOG_F(INFO, "node mText :%s, sel->desc :%s | %ld", node->getText().c_str(),
          sel->mText.c_str(), sel->mText.length());
    LOG_F(INFO, "node mDesc :%s, sel->desc :%s | %ld", node->getDesc().c_str(),
          sel->mDesc.c_str(), sel->mDesc.length());
    LOG_F(INFO, "node mType :%s, sel->type :%s | %ld", node->getType().c_str(),
          sel->mType.c_str(), sel->mType.length());

    return true;
}

PartialMatch::PartialMatch() : mSelector{nullptr}, mDepth{-1}, mPartialMatches{}
{
}

PartialMatch::PartialMatch(const UiSelector *selector, const int absDepth)
    : mSelector{selector}, mDepth{absDepth}, mPartialMatches{}
{
}

std::shared_ptr<PartialMatch> PartialMatch::accept(const AccessibleNode *node,
                                                   const UiSelector *selector,
                                                   int index, int depth)
{
    return PartialMatch::accept(node, selector, index, depth, depth);
}

std::shared_ptr<PartialMatch> PartialMatch::accept(const AccessibleNode *node,
                                                   const UiSelector *selector,
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
    std::set<UiSelector *> matches;
    for (auto match : mPartialMatches) {
        if (match->finalizeMatch()) {
            matches.insert(const_cast<UiSelector *>(match->mSelector));
        }
    }

    for (auto sel : mSelector->mChild) {
        if (!matches.count(sel)) return false;
    }
    return true;
}
