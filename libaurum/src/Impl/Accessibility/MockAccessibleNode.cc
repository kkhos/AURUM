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
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
    const auto trickDontRemove = std::shared_ptr<MockAccessibleNode>( this, [](MockAccessibleNode *){} );

    setProperties(text,pkg,role,res,type,style,automationId, screenBoundingBox, supportingIfaces, featureProperty);
    auto watcher = AccessibleWatcher::getInstance();
    watcher->attach(shared_from_this());
}

MockAccessibleNode::~MockAccessibleNode()
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
    auto watcher = AccessibleWatcher::getInstance();
    watcher->detach(shared_from_this());
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

void* MockAccessibleNode::getRawHandler(void) const
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
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

bool MockAccessibleNode::setFocus()
{
    return false;
}

void MockAccessibleNode::refresh(bool updateAll)
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
