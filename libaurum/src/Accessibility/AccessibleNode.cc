#include "AccessibleNode.h"
#include <string.h>
#include <iostream>
#include <vector>

#include <loguru.hpp>
#include "config.h"
#include <sstream>


AccessibleNode::~AccessibleNode()
{
}

AccessibleNode::AccessibleNode()
: mText{""}, mPkg{""}, mRole{""}, mId{""}, mType{""}, mStyle{""},
  mBoundingBox{0,0,0,0}, mSupportingIfaces(0), mFeatureProperty(0), mValid{true}, mLock{}
{
}

std::string AccessibleNode::description() {
    std::stringstream ss{};
    ss << "{";
    ss << "\"mId\":\"" << this->mId << "\", ";
    ss << "\"mAutomationId\":\"" << this->mAutomationId << "\", ";
    ss << "\"mRole\":\"" << this->mRole << "\", ";
    ss << "\"mText\":\"" << this->mText << "\", ";
    ss << "\"mPkg\":\"" << this->mPkg << "\", ";
    ss << "\"mType\":\"" << this->mType << "\", ";
    ss << "\"mStyle\":\"" << this->mStyle << "\", ";
    /*
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
    */
    ss << "}";
    return ss.str();
}

void AccessibleNode::notify(int type, int type2, void *src)
{
    LOG_SCOPE_F(INFO, "notified for obj(%p) t1:%d t2:%d src:%p",this, type, type2, src);
    void *handler = getRawHandler();

    if ((EventType)type == EventType::Object && (ObjectEventType)type2 == ObjectEventType::ObjectStateDefunct) {
        if (handler == src) invalidate();
    }
}

void AccessibleNode::invalidate()
{
    std::unique_lock<std::mutex> lock(mLock);
    LOG_F(INFO, "object %p is now invalid", this);
    mValid = false;
}

bool AccessibleNode::isValid() const
{
    std::unique_lock<std::mutex> lock(mLock);
    if (!getRawHandler() || !mValid) return false;
    return true;
}

void AccessibleNode::print(int depth, int maxDepth)
{
    if (maxDepth <= 0 || depth > maxDepth) return;

    this->print(depth);
    auto children = this->getChildren();
    for ( auto &child : children ) {
        if (child) child->print(depth +1, maxDepth);
    }
}

void AccessibleNode::print(int d)
{
    this->refresh();
    LOG_F(INFO, "%s%s",std::string(d, ' ').c_str(), description().c_str());
}

bool AccessibleNode::isSupporting(AccessibleNodeInterface thisIface) const
{
    return (mSupportingIfaces & static_cast<int>(thisIface)) != 0;
}

bool AccessibleNode::hasFeatureProperty(NodeFeatureProperties prop) const
{
    return (mFeatureProperty & static_cast<int>(prop)) != 0;
}

void AccessibleNode::setFeatureProperty(NodeFeatureProperties prop, bool has)
{
    if (has)
        mFeatureProperty |= static_cast<int>(prop);
    else
        mFeatureProperty &= ~static_cast<int>(prop);
}


std::string AccessibleNode::getText() const
{
    return mText;
}

std::string AccessibleNode::getPkg() const
{
    return mPkg;
}

std::string AccessibleNode::getId() const
{
    return mId;
}

std::string AccessibleNode::getAutomationId() const
{
    return mAutomationId;
}

std::string AccessibleNode::getRole() const
{
    return mRole;
}

std::string AccessibleNode::getType() const
{
    return mType;
}

std::string AccessibleNode::getStyle() const
{
    return mStyle;
}

Rect<int> AccessibleNode::getBoundingBox() const
{
    return mBoundingBox;
}

bool AccessibleNode::isCheckable() const
{
    return hasFeatureProperty(NodeFeatureProperties::CHECKABLE);
}

bool AccessibleNode::isChecked() const
{
    return hasFeatureProperty(NodeFeatureProperties::CHECKED);
}

bool AccessibleNode::isClickable() const
{
    return hasFeatureProperty(NodeFeatureProperties::CLICKABLE);
}

bool AccessibleNode::isEnabled() const
{
    return hasFeatureProperty(NodeFeatureProperties::ENABLED);
}

bool AccessibleNode::isFocusable() const
{
    return hasFeatureProperty(NodeFeatureProperties::FOCUSABLE);
}

bool AccessibleNode::isFocused() const
{
    return hasFeatureProperty(NodeFeatureProperties::FOCUSED);
}

bool AccessibleNode::isLongClickable() const
{
    return hasFeatureProperty(NodeFeatureProperties::LONGCLICKABLE);
}

bool AccessibleNode::isScrollable() const
{
    return hasFeatureProperty(NodeFeatureProperties::SCROLLABLE);
}

bool AccessibleNode::isSelectable() const
{
    return hasFeatureProperty(NodeFeatureProperties::SELECTABLE);
}

bool AccessibleNode::isSelected() const
{
    return hasFeatureProperty(NodeFeatureProperties::SELECTED);
}

bool AccessibleNode::isVisible() const
{
    return hasFeatureProperty(NodeFeatureProperties::VISIBLE);
}

bool AccessibleNode::isShowing() const
{
    return hasFeatureProperty(NodeFeatureProperties::SHOWING);
}

bool AccessibleNode::isActive() const
{
    return hasFeatureProperty(NodeFeatureProperties::ACTIVE);
}