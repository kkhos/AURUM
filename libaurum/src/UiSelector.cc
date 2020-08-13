#include "UiSelector.h"
#include <string>

UiSelector::UiSelector()
    : mChild{}
{
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
UiSelector *UiSelector::id(std::string text)
{
    this->mId = std::make_unique<std::string>(text);
    return this;
}

UiSelector *UiSelector::text(std::string text)
{
    this->mText = std::make_unique<std::string>(text);
    return this;
}

UiSelector *UiSelector::pkg(std::string text)
{
    this->mPkg = std::make_unique<std::string>(text);
    return this;
}

UiSelector *UiSelector::res(std::string text)
{
    this->mRes = std::make_unique<std::string>(text);
    return this;
}

UiSelector *UiSelector::type(std::string text)
{
    this->mType = std::make_unique<std::string>(text);
    return this;
}

UiSelector *UiSelector::style(std::string text)
{
    this->mStyle = std::make_unique<std::string>(text);
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

UiSelector *UiSelector::hasChild(std::shared_ptr<UiSelector> child)
{
    mChild.push_back(child);
    return this;
}
