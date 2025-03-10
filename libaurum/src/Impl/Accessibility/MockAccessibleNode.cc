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

#include "MockAccessibleNode.h"
#include "AccessibleWatcher.h"

#include <algorithm>
#include <iostream>

using namespace Aurum;
using namespace AurumInternal::Mock;

MockAccessibleNode::MockAccessibleNode(std::shared_ptr<AccessibleNode> parent, std::string text, std::string pkg, std::string role, std::string res, std::string type, std::string style,std::string automationId,  Rect<int> screenBoundingBox, int supportingIfaces,int featureProperty)
: mParentNode(parent), mChildrenList{}, mActionSet{}
{
    const auto trickDontRemove = std::shared_ptr<MockAccessibleNode>( this, [](MockAccessibleNode *){} );

    setProperties(text,pkg,role,res,type,style,automationId, screenBoundingBox, supportingIfaces, featureProperty);
}

MockAccessibleNode::~MockAccessibleNode()
{
}

int MockAccessibleNode::getChildCount() const
{
    return mChildrenList.size();
}

std::shared_ptr<AccessibleNode> MockAccessibleNode::getChildAt(int index) const
{
    return mChildrenList.at(index);
}
std::vector<std::shared_ptr<AccessibleNode>> MockAccessibleNode::getChildren() const
{
    return mChildrenList;
}

std::shared_ptr<AccessibleNode> MockAccessibleNode::getParent() const
{
    return mParentNode;
}

std::vector<std::shared_ptr<AccessibleNode>> MockAccessibleNode::getMatches(const std::shared_ptr<UiSelector> selector, const bool ealryReturn) const
{
    std::vector<std::shared_ptr<AccessibleNode>> ret{};

    if (selector->mMatchText) {
        for (auto &child : mChildrenList) {
            if (selector->mText == child->getText()) {
                ret.push_back(child);
                if (ealryReturn) break;
            }
        }
    }

    if (selector->mMatchType) {
        for (auto &child : mChildrenList) {
            if (selector->mType == child->getType()) {
                ret.push_back(child);
                if (ealryReturn) break;
            }
        }
    }

    if (selector->mMatchStyle) {
        for (auto &child : mChildrenList) {
            if (selector->mStyle == child->getStyle()) {
                ret.push_back(child);
                if (ealryReturn) break;
            }
        }
    }
    return ret;
}

std::vector<std::shared_ptr<AccessibleNode>> MockAccessibleNode::getMatchesInMatches(const std::shared_ptr<UiSelector> firstSelector, const std::shared_ptr<UiSelector> secondSelector, const bool ealryReturn) const
{
    std::vector<std::shared_ptr<AccessibleNode>> tempRet{};
    std::vector<std::shared_ptr<AccessibleNode>> ret{};

    tempRet = getMatches(firstSelector, ealryReturn);

    if (secondSelector->mMatchText) {
        for (auto &child : tempRet) {
            if (secondSelector->mText == child->getText()) {
                ret.push_back(child);
                if (ealryReturn) break;
            }
        }
    }

    if (secondSelector->mMatchType) {
        for (auto &child : tempRet) {
            if (secondSelector->mType == child->getType()) {
                ret.push_back(child);
                if (ealryReturn) break;
            }
        }
    }

    if (secondSelector->mMatchStyle) {
        for (auto &child : tempRet) {
            if (secondSelector->mStyle == child->getStyle()) {
                ret.push_back(child);
                if (ealryReturn) break;
            }
        }
    }

    return ret;
}

std::string MockAccessibleNode::dumpTree() const
{
    return {"{}"};
}

void* MockAccessibleNode::getRawHandler(void) const
{
    return (void*)1;
}

void MockAccessibleNode::setProperties(std::string text,std::string pkg,std::string role,std::string id,std::string type,std::string style,std::string automationId, Rect<int> screenBoundingBox,int supportingIfaces,int featureProperty)
{
    mText = text;
    mPkg = pkg;
    mRole = role;
    mId = id;
    mAutomationId = automationId;
    mType = type;
    mStyle = style;
    mScreenBoundingBox = screenBoundingBox;
    mSupportingIfaces = supportingIfaces;
    mFeatureProperty = featureProperty;
}

void MockAccessibleNode::updateRoleName()
{
}

void MockAccessibleNode::updateUniqueId()
{
}

void MockAccessibleNode::updateName()
{
}

void MockAccessibleNode::updateApplication()
{
}

void MockAccessibleNode::updateAttributes()
{
}

void MockAccessibleNode::updateStates()
{
}

void MockAccessibleNode::updateExtents()
{
}

void MockAccessibleNode::updateXPath()
{
}

void MockAccessibleNode::updateValue()
{
}

void MockAccessibleNode::updatePid()
{
}

void MockAccessibleNode::updateToolkitName()
{
}

void MockAccessibleNode::updateInterface()
{
}

bool MockAccessibleNode::setFocus()
{
    return false;
}

bool MockAccessibleNode::moveTo()
{
    return false;
}

void MockAccessibleNode::updateTextMinBoundingRect()
{
}

void MockAccessibleNode::refresh(bool updateAll)
{
    mText = "refreshText";
    mPkg = "refreshPkg";
    mRole = "refreshRole";
    mId = "refreshId";
    mAutomationId = "refreshAutomationId";
    mType = "refreshType";
    mStyle = "refreshStyle";
    mScreenBoundingBox = {0,0,500,500};
    mSupportingIfaces = 1;
    mFeatureProperty = (int)NodeFeatureProperties::SELECTABLE | (int)NodeFeatureProperties::SELECTED;
}

void MockAccessibleNode::refresh(std::string text, std::string role, std::string type,
                                 std::string automationId, std::string description, std::string imgSrc,
                                 double value, double minValue, double maxValue, double increment, Rect<int> extents)
{
}

std::vector<std::string> MockAccessibleNode::getActions() const
{
    std::vector<std::string> ret{};
    std::transform(mActionSet.begin(), mActionSet.end(), std::back_inserter(ret), [](auto action){
        return action;
    });
    return ret;
}

bool MockAccessibleNode::doAction(std::string action)
{
    if (mActionSet.find(action) != mActionSet.end()) return true;
    return false;
}

bool MockAccessibleNode::setValue(std::string text)
{
    mText = text;
    return true;
}

bool MockAccessibleNode::setValue(double value)
{
    return true;
}

void MockAccessibleNode::setFeatureProperty(int type)
{
    switch(type) {
        case 1:
            setFeatureProperty(NodeFeatureProperties::CHECKED, true);
            break;
        case 2:
            setFeatureProperty(NodeFeatureProperties::CHECKABLE, true);
            break;
        case 3:
            setFeatureProperty(NodeFeatureProperties::ENABLED, true);
            break;
        case 4:
            setFeatureProperty(NodeFeatureProperties::FOCUSABLE, true);
            break;
        case 5:
            setFeatureProperty(NodeFeatureProperties::FOCUSED, true);
            break;
        case 6:
            setFeatureProperty(NodeFeatureProperties::SELECTABLE, true);
            break;
        case 7:
            setFeatureProperty(NodeFeatureProperties::SELECTED, true);
            break;
        case 8:
            setFeatureProperty(NodeFeatureProperties::SHOWING, true);
            break;
        case 9:
            setFeatureProperty(NodeFeatureProperties::VISIBLE, true);
            break;
        case 10:
            setFeatureProperty(NodeFeatureProperties::ACTIVE, true);
            break;
        case 11:
            setFeatureProperty(NodeFeatureProperties::CLICKABLE, true);
            break;
        case 12:
        case 13:
            setFeatureProperty(NodeFeatureProperties::INVALID, true);
            break;
    }
}

void MockAccessibleNode::addChild(std::shared_ptr<AccessibleNode> child)
{
    mChildrenList.push_back(child);
}

std::shared_ptr<MockAccessibleNode> MockAccessibleNode::addChild(std::string text, std::string pkg, std::string role, std::string res, std::string type, std::string style, std::string automationId, Rect<int> geometry, int ifaces, int properties)
{
    auto node = std::make_shared<MockAccessibleNode>(shared_from_this(), text, pkg, role, res, type, style, automationId, geometry, ifaces, properties);
    this->addChild(node);
    return node;
}

void MockAccessibleNode::clearChildren(void)
{
    mChildrenList.clear();
}

void MockAccessibleNode::addAction(std::string action)
{
    mActionSet.insert(action);
}

void MockAccessibleNode::clearActions(void)
{
    mActionSet.clear();
}

std::shared_ptr<AccessibleNode> MockAccessibleNode::next() const
{
    return nullptr;
}

std::shared_ptr<AccessibleNode> MockAccessibleNode::prev() const
{
    return nullptr;
}

std::shared_ptr<AccessibleNode> MockAccessibleNode::first() const
{
    return nullptr;
}

std::shared_ptr<AccessibleNode> MockAccessibleNode::last() const
{
    return nullptr;
}

void MockAccessibleNode::setIncludeHidden(bool enabled) const
{
    return;
}

bool MockAccessibleNode::getIncludeHidden() const
{
    return false;
}

std::shared_ptr<AccessibleNode> MockAccessibleNode::refAccessibleNode(const std::string &appName, const std::string &path) const
{
    return nullptr;
}

