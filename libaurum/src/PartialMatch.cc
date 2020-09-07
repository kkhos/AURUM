#include "PartialMatch.h"

#include <iostream>
#include <set>
#include <regex>

#include <loguru.hpp>

bool PartialMatch::checkCriteria(const std::string *textA, const std::string textB, const bool *match)
{
    if (!textA || !match) return false;
    std::regex re(*textA);
    bool rst = !(!!std::regex_match(textB, re) == (*match));

    LOG_F(INFO, "WKWK# %d %s %s=> %d\n", *match, textA->c_str(), textB.c_str(), rst);
    return rst;
}

bool PartialMatch::checkCriteria(const std::string *textA, const std::string textB)
{
    bool match = true;
    return checkCriteria(textA, textB, &match);
}

bool PartialMatch::checkCriteria(const bool *boolA, const bool boolB)
{
    if (!boolA) return false;
    return *boolA != boolB;
}

void PartialMatch::debugPrint()
{
    if (mSelector->mPkg)
        LOG_F(INFO, "selector->pkg :%s", mSelector->mPkg->c_str());
    if (mSelector->mId)
        LOG_F(INFO, "selector->id :%s", mSelector->mId->c_str());
    if (mSelector->mText)
        LOG_F(INFO, "selector->text :%s", mSelector->mText->c_str());
    if (mSelector->mType)
        LOG_F(INFO, "selector->type :%s", mSelector->mType->c_str());
    if (mSelector->mStyle)
        LOG_F(INFO, "selector->style :%s", mSelector->mStyle->c_str());
}

bool PartialMatch::checkCriteria(const std::shared_ptr<UiSelector> selector,
                                 const std::shared_ptr<AccessibleNode> node)
{
    if(checkCriteria(selector->mText.get(), node->getText(), selector->mMatchText.get())) return false;
    if(checkCriteria(selector->mId.get(), node->getId(), selector->mMatchId.get())) return false;
    if(checkCriteria(selector->mAutomationId.get(), node->getAutomationId(), selector->mMatchAutomationId.get())) return false;
    if(checkCriteria(selector->mType.get(), node->getType(), selector->mMatchType.get())) return false;
    if(checkCriteria(selector->mStyle.get(), node->getStyle(), selector->mMatchStyle.get())) return false;
    if(checkCriteria(selector->mPkg.get(), node->getPkg(), selector->mMatchPkg.get())) return false;
    if(checkCriteria(selector->mRole.get(), node->getRole(), selector->mMatchRole.get())) return false;
    if(checkCriteria(selector->mIschecked.get(), node->isChecked())) return false;
    if(checkCriteria(selector->mIscheckable.get(), node->isCheckable())) return false;
    if(checkCriteria(selector->mIsclickable.get(), node->isClickable())) return false;
    if(checkCriteria(selector->mIsenabled.get(), node->isEnabled())) return false;
    if(checkCriteria(selector->mIsfocused.get(), node->isFocused())) return false;
    if(checkCriteria(selector->mIsfocusable.get(), node->isFocusable())) return false;
    if(checkCriteria(selector->mIsscrollable.get(), node->isScrollable())) return false;
    if(checkCriteria(selector->mIsselected.get(), node->isSelected())) return false;
    if(checkCriteria(selector->mIsshowing.get(), node->isShowing())) return false;
    if(checkCriteria(selector->mIsactive.get(), node->isActive())) return false;
    if(checkCriteria(selector->mIsvisible.get(), node->isVisible())) return false;
    if(checkCriteria(selector->mIsselectable.get(), node->isSelectable())) return false;

    return true;
}

PartialMatch::PartialMatch() : mSelector{nullptr}, mDepth{-1}, mPartialMatches{}
{
}

PartialMatch::PartialMatch(const std::shared_ptr<UiSelector> selector, const int absDepth)
    : mSelector{selector}, mDepth{absDepth}, mPartialMatches{}
{
}

std::shared_ptr<PartialMatch> PartialMatch::accept(const std::shared_ptr<AccessibleNode> node,
                                                   const std::shared_ptr<UiSelector> selector,
                                                   int index, int depth)
{
    return PartialMatch::accept(node, selector, index, depth, depth);
}

std::shared_ptr<PartialMatch> PartialMatch::accept(const std::shared_ptr<AccessibleNode> node,
                                                   const std::shared_ptr<UiSelector> selector,
                                                   int index, int absoluteDepth,
                                                   int relativeDepth)
{
    LOG_SCOPE_F(INFO, "accept checking i:%d a:%d r:%d / %d < %d < %d", index, absoluteDepth, relativeDepth, selector->mMinDepth?*(selector->mMinDepth):-1, relativeDepth, selector->mMaxDepth?*(selector->mMaxDepth):9999999);
    PartialMatch *match = nullptr;

    if ((selector->mMinDepth && (relativeDepth < *(selector->mMinDepth))) ||
        (selector->mMaxDepth && (relativeDepth > *(selector->mMaxDepth)))) {
        LOG_F(INFO, "depth limit overflow %d < %d < %d", selector->mMinDepth?*(selector->mMinDepth):-1, relativeDepth, selector->mMaxDepth?*(selector->mMaxDepth):9999999);
        return std::shared_ptr<PartialMatch>(nullptr);
    }

    if (PartialMatch::checkCriteria(selector, node)) {
        LOG_F(INFO, "New Match found %p %d", selector, absoluteDepth);
        match = new PartialMatch(selector, absoluteDepth);
    }

    return std::shared_ptr<PartialMatch>(match);
}

void PartialMatch::update(
    const std::shared_ptr<AccessibleNode> node, int index, int depth,
    std::list<std::shared_ptr<PartialMatch>> &partialMatches)
{
    for (auto &childSelector : mSelector->mChild) {
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
    for (auto &match : mPartialMatches) {
        if (match->finalizeMatch()) {
            matches.insert(match->mSelector);
        }
    }

    for (auto &sel : mSelector->mChild) {
        if (!matches.count(sel)) return false;
    }
    return true;
}
