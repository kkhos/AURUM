/*
 * Copyright (c) 2022 Samsung Electronics Co., Ltd All Rights Reserved
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

#include <string.h>
#include <iostream>
#include <vector>

#include "config.h"
#include <sstream>

using namespace Aurum;

class Event {
public:

    Event(std::shared_ptr<AccessibleNode> node)
    : mNode{node} {}

    virtual ~Event(){};
    virtual void handleEvent() = 0;
protected:

    std::shared_ptr<AccessibleNode> mNode;
};

class ObjectDefunctEvent : public Event {
public:

    ObjectDefunctEvent(std::shared_ptr<AccessibleNode> node)
    : Event{node} {}

    virtual ~ObjectDefunctEvent(){};
    void handleEvent() override {
        mNode->invalidate();
    }
};

AccessibleNode::~AccessibleNode()
{
}

AccessibleNode::AccessibleNode()
: mText{""}, mOcrText{""}, mPkg{""}, mRole{""}, mId{""}, mAutomationId{""}, mType{""}, mStyle{""}, mXPath{""}, mToolkitName{""}, mInterface{""}, mDescription{""},
  mScreenBoundingBox{0,0,0,0}, mWindowBoundingBox{0,0,0,0}, mTextMinBoundingRect{0,0,0,0}, mSupportingIfaces(0), mFeatureProperty(0), mPid(0), mWindowAngle(0), mTargetAngle(0), mMinValue{0.0}, mMaxValue{0.0}, mValue{0.0}, mIncrement{0.0}, mLayerZIndex{0.0}, mValid{true}, mLock{}
{
}

std::string AccessibleNode::description() {
    std::stringstream ss{};

    ss << "{";
    ss << "\"mId\":\"" << this->mId << "\", ";
    ss << "\"mAutomationId\":\"" << this->mAutomationId << "\", ";
    ss << "\"mRole\":\"" << this->mRole << "\", ";
    ss << "\"mText\":\"" << this->mText << "\", ";
    ss << "\"mDescription\":\"" << this->mDescription << "\", ";
    ss << "\"mOcrText\":\"" << this->mOcrText << "\", ";
    ss << "\"mPkg\":\"" << this->mPkg << "\", ";
    ss << "\"mType\":\"" << this->mType << "\", ";
    ss << "\"mStyle\":\"" << this->mStyle << "\", ";
    ss << "}";

    return ss.str();
}

void AccessibleNode::notify(EventType type, void *src)
{
    if (src != getRawHandler()) return;

    std::unique_ptr<Event> event;
    if (type == EventType::ObjectDefunct)
        event = std::make_unique<ObjectDefunctEvent>(this->shared_from_this());

    event->handleEvent();
}

void AccessibleNode::invalidate()
{
    std::unique_lock<std::mutex> lock(mLock);
    LOGI("object %p is now invalid", this);
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
    LOGI("%s %s",std::string(d, ' ').c_str(), description().c_str());
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

void AccessibleNode::resetFeatureProperty()
{
    mFeatureProperty = 0;
}

std::string AccessibleNode::getText() const
{
    return mText;
}

std::string AccessibleNode::getOcrText() const
{
    return mOcrText;
}

int AccessibleNode::getWindowAngle() const
{
    return mWindowAngle;
}

int AccessibleNode::getTargetAngle() const
{
    return mTargetAngle;
}

void AccessibleNode::setOcrText(std::string text)
{
    mOcrText = text;
}

void AccessibleNode::setWindowAngle(int windowAngle)
{
    mWindowAngle = windowAngle;
}

void AccessibleNode::setTargetAngle(int targetAngle)
{
    mTargetAngle = targetAngle;
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

std::string AccessibleNode::getXPath() const
{
    return mXPath;
}

std::string AccessibleNode::getToolkitName() const
{
    return mToolkitName;
}

const Rect<int> AccessibleNode::getScreenBoundingBox() const
{
    return mScreenBoundingBox;
}

const Rect<int> AccessibleNode::getWindowBoundingBox() const
{
    return mWindowBoundingBox;
}

std::string AccessibleNode::getInterface() const
{
    return mInterface;
}

std::string AccessibleNode::getDescription() const
{
    return mDescription;
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

bool AccessibleNode::isHighlightable() const
{
    return hasFeatureProperty(NodeFeatureProperties::HIGHLIGHTABLE);
}

double AccessibleNode::getMinValue() const
{
    return mMinValue;
}

double AccessibleNode::getMaxValue() const
{
    return mMaxValue;
}

double AccessibleNode::getValue() const
{
    return mValue;
}

double AccessibleNode::getIncrement() const
{
    return mIncrement;
}

int AccessibleNode::getPid() const
{
    return mPid;
}

Rect<int> AccessibleNode::getTextMinBoundingRect() const
{
    return mTextMinBoundingRect;
}
