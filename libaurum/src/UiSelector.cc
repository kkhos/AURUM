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
#include <string>
#include <sstream>

using namespace Aurum;

UiSelector::UiSelector()
: mId{}, mAutomationId{}, mRole{}, mText{}, mPkg{}, mType{}, mStyle{}, mTextPartialMatch{}, mXPath{}, mOcrText{},
  mMatchId{}, mMatchAutomationId{}, mMatchRole{}, mMatchText{}, mMatchPkg{}, mMatchType{}, mMatchStyle{},
  mMatchTextPartialMatch{}, mMatchXPath{}, mMatchOcrText{}, mMatchChecked{}, mMatchCheckable{}, mMatchClickable{}, mMatchEnabled{},
  mMatchFocused{}, mMatchFocusable{}, mMatchScrollable{}, mMatchSelected{}, mMatchShowing{}, mMatchActive{}, mMatchVisible{},
  mMatchSelectable{}, mMinDepth{}, mMaxDepth{}, mIschecked{}, mIscheckable{}, mIsclickable{}, mIsenabled{},
  mIsfocused{}, mIsfocusable{}, mIsscrollable{}, mIsselected{}, mIsshowing{}, mIsactive{}, mIsvisible{},
  mIsselectable{}, mChild{}, mParent{}
{
}

std::string UiSelector::description()
{
    std::stringstream ss{};
    ss << "{";
    if(!this->mId.empty()) ss << "\"mId\":\"" << this->mId << "\", ";
    if(!this->mAutomationId.empty()) ss << "\"mAutomationId\":\"" << this->mAutomationId << "\", ";
    if(!this->mRole.empty()) ss << "\"mRole\":\"" << this->mRole << "\", ";
    if(!this->mText.empty()) ss << "\"mText\":\"" << this->mText << "\", ";
    if(!this->mOcrText.empty()) ss << "\"mOcrText\":\"" << this->mOcrText << "\", ";
    if(!this->mTextPartialMatch.empty()) ss << "\"mTextPartialMatch\":\"" << this->mTextPartialMatch << "\", ";
    if(!this->mXPath.empty()) ss << "\"mXPath\":\"" << this->mXPath << "\", ";
    if(!this->mPkg.empty()) ss << "\"mPkg\":\"" << this->mPkg << "\", ";
    if(!this->mType.empty()) ss << "\"mType\":\"" << this->mType << "\", ";
    if(!this->mStyle.empty()) ss << "\"mStyle\":\"" << this->mStyle << "\", ";
    if(this->mMatchId) ss << "\"mMatchId\":\"" << ((this->mMatchId)?"true":"false") << "\", ";
    if(this->mMatchAutomationId) ss << "\"mMatchAutomationId\":\"" << ((this->mMatchAutomationId)?"true":"false") << "\", ";
    if(this->mMatchRole) ss << "\"mMatchRole\":\"" << ((this->mMatchRole)?"true":"false") << "\", ";
    if(this->mMatchText) ss << "\"mMatchText\":\"" << ((this->mMatchText)?"true":"false") << "\", ";
    if(this->mMatchOcrText) ss << "\"mMatchOcrText\":\"" << ((this->mMatchOcrText)?"true":"false") << "\", ";
    if(this->mMatchTextPartialMatch) ss << "\"mMatchTextPartialMatch\":\"" << ((this->mMatchTextPartialMatch)?"true":"false") << "\", ";
    if(this->mMatchXPath) ss << "\"mMatchXPath\":\"" << ((this->mMatchXPath)?"true":"false") << "\", ";
    if(this->mMatchPkg) ss << "\"mMatchPkg\":\"" << ((this->mMatchPkg)?"true":"false") << "\", ";
    if(this->mMatchType) ss << "\"mMatchType\":\"" << ((this->mMatchType)?"true":"false") << "\", ";
    if(this->mMatchStyle) ss << "\"mMatchStyle\":\"" << ((this->mMatchStyle)?"true":"false" )<< "\", ";
    if(this->mMinDepth) ss << "\"mMinDepth\":\"" << this->mMinDepth << "\", ";
    if(this->mMaxDepth) ss << "\"mMaxDepth\":\"" << this->mMaxDepth << "\", ";
    if(this->mMatchChecked) ss << "\"mMatchChecked\":\"" << ((this->mMatchChecked)?"true":"false") << "\", ";
    if(this->mMatchCheckable) ss << "\"mMatchCheckable\":\"" << ((this->mMatchCheckable)?"true":"false") << "\", ";
    if(this->mMatchClickable) ss << "\"mMatchClickable\":\"" << ((this->mMatchClickable)?"true":"false") << "\", ";
    if(this->mMatchEnabled) ss << "\"mMatchEnabled\":\"" << ((this->mMatchEnabled)?"true":"false") << "\", ";
    if(this->mMatchFocused) ss << "\"mMatchFocused\":\"" << ((this->mMatchFocused)?"true":"false") << "\", ";
    if(this->mMatchFocusable) ss << "\"mMatchFocusable\":\"" << ((this->mMatchFocusable)?"true":"false") << "\", ";
    if(this->mMatchScrollable) ss << "\"mMatchScrollable\":\"" << ((this->mMatchScrollable)?"true":"false") << "\", ";
    if(this->mMatchSelected) ss << "\"mMatchSelected\":\"" << ((this->mMatchSelected)?"true":"false") << "\", ";
    if(this->mMatchShowing) ss << "\"mMatchShowing\":\"" << ((this->mMatchShowing)?"true":"false") << "\", ";
    if(this->mMatchActive) ss << "\"mMatchActive\":\"" << ((this->mMatchActive)?"true":"false") << "\", ";
    if(this->mMatchVisible) ss << "\"mMatchVisible\":\"" << ((this->mMatchVisible)?"true":"false") << "\", ";
    if(this->mMatchSelectable) ss << "\"mMatchSelectable\":\"" << ((this->mMatchSelectable)?"true":"false") << "\", ";
    if(this->mParent) {
        ss << "\"mParent\":" << this->mParent->description();
    }
    ss << "\"mChild\":[";
    for ( auto child = mChild.begin(); child != mChild.end(); child++) {
        ss << (*child)->description();
        if (child+1 != mChild.end()) ss << ", " ;
    }
    ss << "]";

    ss << "}";
    return ss.str();
}

UiSelector *UiSelector::text(std::string text)
{
    this->mText = text;
    this->mMatchText = true;
    return this;
}

UiSelector *UiSelector::ocrText(std::string text)
{
    this->mOcrText = text;
    this->mMatchOcrText = true;
    return this;
}

UiSelector *UiSelector::textPartialMatch(std::string text)
{
    this->mTextPartialMatch = text;
    this->mMatchTextPartialMatch = true;
    return this;
}

UiSelector *UiSelector::pkg(std::string text)
{
    this->mPkg = text;
    this->mMatchPkg = true;
    return this;
}

UiSelector *UiSelector::id(std::string text)
{
    this->mId = text;
    this->mMatchId = true;
    return this;
}

UiSelector *UiSelector::automationid(std::string text)
{
    this->mAutomationId = text;
    this->mMatchAutomationId = true;
    return this;
}

UiSelector *UiSelector::xpath(std::string xpath)
{
    this->mXPath = xpath;
    this->mMatchXPath = true;
    return this;
}

UiSelector *UiSelector::role(std::string text)
{
    this->mRole = text;
    this->mMatchRole = true;
    return this;
}

UiSelector *UiSelector::type(std::string text)
{
    this->mType = text;
    this->mMatchType = true;
    return this;
}

UiSelector *UiSelector::style(std::string text)
{
    this->mStyle = text;
    this->mMatchStyle = true;
    return this;
}

UiSelector *UiSelector::depth(int depth)
{
    this->mMinDepth = depth;
    this->mMaxDepth = depth;
    return this;
}

UiSelector *UiSelector::depth(int minDepth, int maxDepth)
{
    this->mMinDepth = minDepth;
    this->mMaxDepth = maxDepth;
    return this;
}

UiSelector *UiSelector::minDepth(int depth)
{
    this->mMinDepth = depth;
    return this;
}

UiSelector *UiSelector::maxDepth(int depth)
{
    this->mMaxDepth = depth;
    return this;
}

UiSelector *UiSelector::isChecked(bool condition)
{
    this->mIschecked = condition;
    this->mMatchChecked = true;
    return this;
}

UiSelector *UiSelector::isCheckable(bool condition)
{
    this->mIscheckable = condition;
    this->mMatchCheckable = true;
    return this;
}

UiSelector *UiSelector::isClickable(bool condition)
{
    this->mIsclickable = condition;
    this->mMatchClickable = true;
    return this;
}

UiSelector *UiSelector::isEnabled(bool condition)
{
    this->mIsenabled = condition;
    this->mMatchEnabled = true;
    return this;
}

UiSelector *UiSelector::isFocused(bool condition)
{
    this->mIsfocused = condition;
    this->mMatchFocused = true;
    return this;
}

UiSelector *UiSelector::isFocusable(bool condition)
{
    this->mIsfocusable = condition;
    this->mMatchFocusable = true;
    return this;
}

UiSelector *UiSelector::isScrollable(bool condition)
{
    this->mIsscrollable = condition;
    this->mMatchScrollable = true;
    return this;
}

UiSelector *UiSelector::isSelected(bool condition)
{
    this->mIsselected = condition;
    this->mMatchSelected = true;
    return this;
}

UiSelector *UiSelector::isShowing(bool condition)
{
    this->mIsshowing = condition;
    this->mMatchShowing = true;
    return this;
}

UiSelector *UiSelector::isActive(bool condition)
{
    this->mIsactive = condition;
    this->mMatchActive = true;
    return this;
}

UiSelector *UiSelector::isVisible(bool condition)
{
    this->mIsvisible = condition;
    this->mMatchVisible = true;
    return this;
}

UiSelector *UiSelector::isSelectable(bool condition)
{
    this->mIsselectable = condition;
    this->mMatchSelectable = true;
    return this;
}

UiSelector *UiSelector::hasChild(std::shared_ptr<UiSelector> child)
{
    mChild.push_back(child);
    return this;
}

UiSelector *UiSelector::fromParent(std::shared_ptr<UiSelector> parent)
{
    mParent = parent;
    return this;
}
