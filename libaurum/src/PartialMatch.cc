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

#include "Aurum.h"

#include <iostream>
#include <set>
#include <regex>
#include <sstream>

using namespace Aurum;

bool PartialMatch::checkCriteria(const std::string textA, const std::string textB, const bool textPartialMatch)
{
    if (textB.empty()) return true;

    bool rst;
    if (textPartialMatch) {
        if (textB.find(textA) != std::string::npos) rst = false;
        else rst = true;
    }
    else {
        if (!textA.compare(textB)) rst = false;
        else rst = true;
    }

    return rst;
}

bool PartialMatch::checkCriteria(const bool boolA, const bool boolB)
{
    return boolA != boolB;
}

std::string PartialMatch::debugPrint()
{
    return mSelector->description();
}

bool PartialMatch::checkCriteria(const std::shared_ptr<UiSelector> selector,
                                 const std::shared_ptr<AccessibleNode> node)
{
    if (selector->mMatchText || selector->mMatchTextPartialMatch) {
        node->updateName();
        if (selector->mMatchText && checkCriteria(selector->mText, node->getText(), 0)) return false;
        if (selector->mMatchTextPartialMatch && checkCriteria(selector->mTextPartialMatch, node->getText(), 1)) return false;
    }
    if (selector->mMatchId) {
        node->updateUniqueId();
        if (checkCriteria(selector->mId, node->getId(), 0)) return false;
    }
    if (selector->mMatchType || selector->mMatchAutomationId || selector->mMatchStyle) {
        node->updateAttributes();
        if (selector->mMatchAutomationId && checkCriteria(selector->mAutomationId, node->getAutomationId(), 0)) return false;
        if (selector->mMatchType && checkCriteria(selector->mType, node->getType(), 0)) return false;
        if (selector->mMatchStyle && checkCriteria(selector->mStyle, node->getStyle(), 0)) return false;
    }
    if (selector->mMatchPkg) {
        node->updateApplication();
         if (checkCriteria(selector->mPkg, node->getPkg(), 0)) return false;
    }
    if (selector->mMatchRole) {
        node->updateRoleName();
        if (checkCriteria(selector->mRole, node->getRole(), 0)) return false;
    }
    if (selector->mMatchChecked && checkCriteria(selector->mIschecked, node->isChecked())) return false;
    if (selector->mMatchCheckable && checkCriteria(selector->mIscheckable, node->isCheckable())) return false;
    if (selector->mMatchClickable && checkCriteria(selector->mIsclickable, node->isClickable())) return false;
    if (selector->mMatchEnabled && checkCriteria(selector->mIsenabled, node->isEnabled())) return false;
    if (selector->mMatchFocused && checkCriteria(selector->mIsfocused, node->isFocused())) return false;
    if (selector->mMatchFocusable && checkCriteria(selector->mIsfocusable, node->isFocusable())) return false;
    if (selector->mMatchScrollable && checkCriteria(selector->mIsscrollable, node->isScrollable())) return false;
    if (selector->mMatchSelected && checkCriteria(selector->mIsselected, node->isSelected())) return false;
    if (selector->mMatchShowing && checkCriteria(selector->mIsshowing, node->isShowing())) return false;
    if (selector->mMatchActive && checkCriteria(selector->mIsactive, node->isActive())) return false;
    if (selector->mMatchVisible && checkCriteria(selector->mIsvisible, node->isVisible())) return false;
    if (selector->mMatchSelectable && checkCriteria(selector->mIsselectable, node->isSelectable())) return false;

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
    PartialMatch *match = nullptr;

    if ((selector->mMinDepth && relativeDepth < selector->mMinDepth) ||
        (selector->mMaxDepth && relativeDepth > selector->mMaxDepth)) {
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
