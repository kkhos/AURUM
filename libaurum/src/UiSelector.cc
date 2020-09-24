#include "UiSelector.h"
#include <string>
#include <sstream>

UiSelector::UiSelector()
: mId{}, mAutomationId{}, mRole{}, mText{}, mPkg{}, mType{}, mStyle{},
  mMatchId{}, mMatchAutomationId{}, mMatchRole{}, mMatchText{}, mMatchPkg{}, mMatchType{}, mMatchStyle{},
  mMinDepth{}, mMaxDepth{}, mIschecked{}, mIscheckable{}, mIsclickable{},
  mIsenabled{}, mIsfocused{}, mIsfocusable{}, mIsscrollable{}, mIsselected{},
  mIsshowing{}, mIsactive{}, mIsvisible{}, mIsselectable{},
  mChild{}, mParent{}
{
}

std::string UiSelector::description()
{
    std::stringstream ss{};
    ss << "{";
    if(this->mId) ss << "\"mId\":\"" << *this->mId << "\", ";
    if(this->mAutomationId) ss << "\"mAutomationId\":\"" << *this->mAutomationId << "\", ";
    if(this->mRole) ss << "\"mRole\":\"" << *this->mRole << "\", ";
    if(this->mText) ss << "\"mText\":\"" << *this->mText << "\", ";
    if(this->mPkg) ss << "\"mPkg\":\"" << *this->mPkg << "\", ";
    if(this->mType) ss << "\"mType\":\"" << *this->mType << "\", ";
    if(this->mStyle) ss << "\"mStyle\":\"" << *this->mStyle << "\", ";
    if(this->mMatchId) ss << "\"mMatchId\":\"" << ((*this->mMatchId)?"true":"false") << "\", ";
    if(this->mMatchAutomationId) ss << "\"mMatchAutomationId\":\"" << ((*this->mMatchAutomationId)?"true":"false") << "\", ";
    if(this->mMatchRole) ss << "\"mMatchRole\":\"" << ((*this->mMatchRole)?"true":"false") << "\", ";
    if(this->mMatchText) ss << "\"mMatchText\":\"" << ((*this->mMatchText)?"true":"false") << "\", ";
    if(this->mMatchPkg) ss << "\"mMatchPkg\":\"" << ((*this->mMatchPkg)?"true":"false") << "\", ";
    if(this->mMatchType) ss << "\"mMatchType\":\"" << ((*this->mMatchType)?"true":"false") << "\", ";
    if(this->mMatchStyle) ss << "\"mMatchStyle\":\"" << ((*this->mMatchStyle)?"true":"false" )<< "\", ";
    if(this->mMinDepth) ss << "\"mMinDepth\":\"" << *this->mMinDepth << "\", ";
    if(this->mMaxDepth) ss << "\"mMaxDepth\":\"" << *this->mMaxDepth << "\", ";
    if(this->mIschecked) ss << "\"mIschecked\":\"" << ((*this->mIschecked)?"true":"false") << "\", ";
    if(this->mIscheckable) ss << "\"mIscheckable\":\"" << ((*this->mIscheckable)?"true":"false") << "\", ";
    if(this->mIsclickable) ss << "\"mIsclickable\":\"" << ((*this->mIsclickable)?"true":"false") << "\", ";
    if(this->mIsenabled) ss << "\"mIsenabled\":\"" << ((*this->mIsenabled)?"true":"false") << "\", ";
    if(this->mIsfocused) ss << "\"mIsfocused\":\"" << ((*this->mIsfocused)?"true":"false") << "\", ";
    if(this->mIsfocusable) ss << "\"mIsfocusable\":\"" << ((*this->mIsfocusable)?"true":"false") << "\", ";
    if(this->mIsscrollable) ss << "\"mIsscrollable\":\"" << ((*this->mIsscrollable)?"true":"false") << "\", ";
    if(this->mIsselected) ss << "\"mIsselected\":\"" << ((*this->mIsselected)?"true":"false") << "\", ";
    if(this->mIsshowing) ss << "\"mIsshowing\":\"" << ((*this->mIsshowing)?"true":"false") << "\", ";
    if(this->mIsactive) ss << "\"mIsactive\":\"" << ((*this->mIsactive)?"true":"false") << "\", ";
    if(this->mIsvisible) ss << "\"mIsvisible\":\"" << ((*this->mIsvisible)?"true":"false") << "\", ";
    if(this->mIsselectable) ss << "\"mIsselectable\":\"" << ((*this->mIsselectable)?"true":"false") << "\", ";
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

/*
UiSelector::UiSelector(const UiSelector &src)
{
    // 복사 생성자
}

UiSelector& UiSelector::operator= (const UiSelector& src)
{
    // = 오버라이드
    if (this == &src) return *this; // 자기 대입 방지
    return *this;
}
*/

UiSelector *UiSelector::text(std::string text, bool match)
{
    this->mText = std::make_unique<std::string>(text);
    this->mMatchText = std::make_unique<bool>(match);
    return this;
}

UiSelector *UiSelector::pkg(std::string text, bool match)
{
    this->mPkg = std::make_unique<std::string>(text);
    this->mMatchPkg = std::make_unique<bool>(match);
    return this;
}

UiSelector *UiSelector::id(std::string text, bool match)
{
    this->mId = std::make_unique<std::string>(text);
    this->mMatchId = std::make_unique<bool>(match);
    return this;
}

UiSelector *UiSelector::automationid(std::string text, bool match)
{
    this->mAutomationId = std::make_unique<std::string>(text);
    this->mMatchAutomationId = std::make_unique<bool>(match);
    return this;
}

UiSelector *UiSelector::role(std::string text, bool match)
{
    this->mRole = std::make_unique<std::string>(text);
    this->mMatchRole = std::make_unique<bool>(match);
    return this;
}

UiSelector *UiSelector::type(std::string text, bool match)
{
    this->mType = std::make_unique<std::string>(text);
    this->mMatchType = std::make_unique<bool>(match);
    return this;
}

UiSelector *UiSelector::style(std::string text, bool match)
{
    this->mStyle = std::make_unique<std::string>(text);
    this->mMatchStyle = std::make_unique<bool>(match);
    return this;
}

UiSelector *UiSelector::depth(int depth)
{
    this->mMinDepth = std::make_unique<int>(depth);
    this->mMaxDepth = std::make_unique<int>(depth);
    return this;
}

UiSelector *UiSelector::depth(int minDepth, int maxDepth)
{
    this->mMinDepth = std::make_unique<int>(minDepth);
    this->mMaxDepth = std::make_unique<int>(maxDepth);
    return this;
}

UiSelector *UiSelector::minDepth(int depth)
{
    this->mMinDepth = std::make_unique<int>(depth);
    return this;
}

UiSelector *UiSelector::maxDepth(int depth)
{
    this->mMaxDepth = std::make_unique<int>(depth);
    return this;
}

UiSelector *UiSelector::isChecked(bool cond)
{
    this->mIschecked = std::make_unique<bool>(cond);
    return this;
}

UiSelector *UiSelector::isCheckable(bool cond)
{
    this->mIscheckable = std::make_unique<bool>(cond);
    return this;
}

UiSelector *UiSelector::isClickable(bool cond)
{
    this->mIsclickable = std::make_unique<bool>(cond);
    return this;
}

UiSelector *UiSelector::isEnabled(bool cond)
{
    this->mIsenabled = std::make_unique<bool>(cond);
    return this;
}

UiSelector *UiSelector::isFocused(bool cond)
{
    this->mIsfocused = std::make_unique<bool>(cond);
    return this;
}

UiSelector *UiSelector::isFocusable(bool cond)
{
    this->mIsfocusable = std::make_unique<bool>(cond);
    return this;
}

UiSelector *UiSelector::isScrollable(bool cond)
{
    this->mIsscrollable = std::make_unique<bool>(cond);
    return this;
}

UiSelector *UiSelector::isSelected(bool cond)
{
    this->mIsselected = std::make_unique<bool>(cond);
    return this;
}

UiSelector *UiSelector::isShowing(bool cond)
{
    this->mIsshowing = std::make_unique<bool>(cond);
    return this;
}

UiSelector *UiSelector::isActive(bool cond)
{
    this->mIsactive = std::make_unique<bool>(cond);
    return this;
}

UiSelector *UiSelector::isVisible(bool cond)
{
    this->mIsvisible = std::make_unique<bool>(cond);
    return this;
}

UiSelector *UiSelector::isSelectable(bool cond)
{
    this->mIsselectable = std::make_unique<bool>(cond);
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