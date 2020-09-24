#include "PartialMatch.h"

#include <iostream>
#include <set>
#include <regex>
#include <sstream>

#include <loguru.hpp>


bool PartialMatch::checkCriteria(const std::string *textA, const std::string textB, const bool *match)
{
    if (!textA || !match) return false;
    std::regex re(*textA);
    bool rst = !(!!std::regex_match(textB, re) == (*match));
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

std::string PartialMatch::debugPrint()
{
    return mSelector->description();
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
    //LOG_SCOPE_F(INFO, "PartialMatch::accept idx:%d abs:%d rel:%d / %d < %d < %d", index, absoluteDepth, relativeDepth, selector->mMinDepth?*(selector->mMinDepth):-1, relativeDepth, selector->mMaxDepth?*(selector->mMaxDepth):9999999);
    PartialMatch *match = nullptr;

    if ((selector->mMinDepth && (relativeDepth < *(selector->mMinDepth))) ||
        (selector->mMaxDepth && (relativeDepth > *(selector->mMaxDepth)))) {
        return std::shared_ptr<PartialMatch>(nullptr);
    }
    if (PartialMatch::checkCriteria(selector, node))
        match = new PartialMatch(selector, absoluteDepth);
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
