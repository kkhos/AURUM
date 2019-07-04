#include "UiSelector.h"
#include <string>

UiSelector::UiSelector()
    : mText{""}, mPkg{""}, mRes{""}, mDesc{""}, mType{""}, mDepth{-1}, mChild{}
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

UiSelector *UiSelector::desc(const std::string &text)
{
    this->mDesc = text;
    return this;
}

UiSelector *UiSelector::text(const std::string &text)
{
    this->mText = text;
    return this;
}

UiSelector *UiSelector::pkg(const std::string &text)
{
    this->mPkg = text;
    return this;
}

UiSelector *UiSelector::res(const std::string &text)
{
    this->mRes = text;
    return this;
}

UiSelector *UiSelector::type(const std::string &text)
{
    this->mType = text;
    return this;
}

UiSelector *UiSelector::depth(int depth)
{
    this->mDepth = depth;
    return this;
}

UiSelector *UiSelector::hasChild(UiSelector *child)
{
    mChild.push_back(child);
    return this;
}
