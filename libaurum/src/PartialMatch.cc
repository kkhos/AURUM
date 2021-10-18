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

bool PartialMatch::checkCriteria(const std::string *textA, const std::string textB)
{
    if (!textA) return false;
    std::regex re(*textA);
    bool rst = !(!!std::regex_match(textB, re) == true);
    return rst;
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
    if (selector->mMatchText.get()) {
        node->updateName();
        if (checkCriteria(selector->mText.get(), node->getText())) return false;
    }
    if (selector->mMatchId.get()) {
        node->updateUniqueId();
        if (checkCriteria(selector->mId.get(), node->getId())) return false;
    }
    if (selector->mMatchType.get() || selector->mMatchAutomationId.get() || selector->mMatchStyle.get()) {
        node->updateAttributes();
        if (checkCriteria(selector->mAutomationId.get(), node->getAutomationId())) return false;
        if (checkCriteria(selector->mType.get(), node->getType())) return false;
        if (checkCriteria(selector->mStyle.get(), node->getStyle())) return false;
    }
    if (selector->mMatchPkg.get()) {
        node->updateApplication();
         if (checkCriteria(selector->mPkg.get(), node->getPkg())) return false;
    }
    if (selector->mMatchRole.get()) {
        node->updateRoleName();
        if (checkCriteria(selector->mRole.get(), node->getRole())) return false;
    }
    if (checkCriteria(selector->mIschecked.get(), node->isChecked())) return false;
    if (checkCriteria(selector->mIscheckable.get(), node->isCheckable())) return false;
    if (checkCriteria(selector->mIsclickable.get(), node->isClickable())) return false;
    if (checkCriteria(selector->mIsenabled.get(), node->isEnabled())) return false;
    if (checkCriteria(selector->mIsfocused.get(), node->isFocused())) return false;
    if (checkCriteria(selector->mIsfocusable.get(), node->isFocusable())) return false;
    if (checkCriteria(selector->mIsscrollable.get(), node->isScrollable())) return false;
    if (checkCriteria(selector->mIsselected.get(), node->isSelected())) return false;
    if (checkCriteria(selector->mIsshowing.get(), node->isShowing())) return false;
    if (checkCriteria(selector->mIsactive.get(), node->isActive())) return false;
    if (checkCriteria(selector->mIsvisible.get(), node->isVisible())) return false;
    if (checkCriteria(selector->mIsselectable.get(), node->isSelectable())) return false;

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
