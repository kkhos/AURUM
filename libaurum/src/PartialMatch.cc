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

bool PartialMatch::checkCriteria(const std::string textA, const std::string textB)
{
    std::regex re(textA);
    bool rst = !(!!std::regex_match(textB, re) == true);
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
    if (selector->mMatchText) {
        node->updateName();
        if (checkCriteria(selector->mText, node->getText())) return false;   
    }
    if (selector->mMatchId) {
        node->updateUniqueId();
        if (checkCriteria(selector->mId, node->getId())) return false;
    }
    if (selector->mMatchType || selector->mMatchAutomationId || selector->mMatchStyle) {
        node->updateAttributes();
        if (checkCriteria(selector->mAutomationId, node->getAutomationId())) return false;
        if (checkCriteria(selector->mType, node->getType())) return false;
        if (checkCriteria(selector->mStyle, node->getStyle())) return false;
    }
    if (selector->mMatchPkg) {
        node->updateApplication();
         if (checkCriteria(selector->mPkg, node->getPkg())) return false;
    }
    if (selector->mMatchRole) {
        node->updateRoleName();
        if (checkCriteria(selector->mRole, node->getRole())) return false;
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
