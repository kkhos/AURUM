#include "MockAccessibleNode.h"

#include <algorithm>
#include <iostream>

MockAccessibleNode::MockAccessibleNode(std::shared_ptr<MockAccessibleNode> parent, std::string text, std::string pkg, std::string role, std::string res, std::string type, std::string style,std::string automationId,  Rect<int> boundingBox, int supportingIfaces,int featureProperty)
: mParentNode(parent), mChildrenList{}, mActionSet{}, mLock{}
{
    setProperties(text,pkg,role,res,type,style,automationId, boundingBox, supportingIfaces, featureProperty);
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

void* MockAccessibleNode::getRawHandler(void) const
{
    return (void*)0;
}

void MockAccessibleNode::setProperties(std::string text,std::string pkg,std::string role,std::string id,std::string type,std::string style,std::string automationId, Rect<int> boundingBox,int supportingIfaces,int featureProperty)
{
    mText = text;
    mPkg = pkg;
    mRole = role;
    mId = id;
    mAutomationId = automationId;
    mType = type;
    mStyle = style;
    mBoundingBox = boundingBox;
    mSupportingIfaces = supportingIfaces;
    mFeatureProperty = featureProperty;
}

void MockAccessibleNode::refresh()
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

void MockAccessibleNode::setValue(std::string text)
{
    mText = text;
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
    std::unique_lock<std::mutex> lock(mLock);

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