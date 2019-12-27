#include "UiObject.h"
#include "Comparer.h"
#include "Sel.h"

#include <iostream>
#include <utility>

#include "loguru.hpp"

UiObject::UiObject() : UiObject(nullptr, nullptr, nullptr) {}

UiObject::~UiObject()
{
    if (mWaiter) delete mWaiter;
}

UiObject::UiObject(const UiDevice *device, const UiSelector *selector,
                   const AccessibleNode *node)
    : mDevice(device),
      mSelector(selector),
      mNode(node),
      mWaiter(new Waiter{this, this})
{
    // tood interface to interact with input interface
    // mInputImple = mDevice->getInputInterface();
}

UiObject::UiObject(const UiObject &src)
    : mDevice(src.mDevice),
      mSelector(src.mSelector),
      mNode(src.mNode),
      mWaiter{src.mWaiter}
{
}

UiObject::UiObject(UiObject &&src)
    : mDevice(src.mDevice),
      mSelector(src.mSelector),
      mNode(src.mNode),
      mWaiter{src.mWaiter}
{
    src.mDevice = nullptr;
    src.mSelector = nullptr;
    src.mNode = nullptr;
    src.mWaiter = nullptr;
}

bool UiObject::hasObject(const UiSelector *selector) const
{
    AccessibleNode *node =
        Comparer::findObject(mDevice, selector, getAccessibleNode());
    if (node != nullptr) {
        // todo : what is this node.recycle()
        return true;
    }
    return false;
}

std::unique_ptr<UiObject> UiObject::findObject(const UiSelector *selector) const
{
    AccessibleNode *node =
        Comparer::findObject(mDevice, selector, getAccessibleNode());
    if (node)
        return std::make_unique<UiObject>(mDevice, selector, node);
    else
        return std::unique_ptr<UiObject>{nullptr};
}

std::vector<std::unique_ptr<UiObject>> UiObject::findObjects(
    const UiSelector *selector) const
{
    return std::vector<std::unique_ptr<UiObject>>{};
}

bool UiObject::waitFor(
    const std::function<bool(const ISearchable *)> condition) const
{
    return mWaiter->waitFor(condition);
}

std::unique_ptr<UiObject> UiObject::waitFor(
    const std::function<std::unique_ptr<UiObject>(const ISearchable *)>
        condition) const
{
    return mWaiter->waitFor(condition);
}

bool UiObject::waitFor(
    const std::function<bool(const UiObject *)> condition) const
{
    LOG_F(INFO, "asdf");
    return mWaiter->waitFor(condition);
}

UiObject *UiObject::getParent() const
{
    AccessibleNode *node = getAccessibleNode()->getParent();
    if (!node) return nullptr;
    return new UiObject(mDevice, mSelector, node);
}

int UiObject::getChildCount() const
{
    return getAccessibleNode()->getChildCount();
}

std::vector<std::unique_ptr<UiObject>> UiObject::getChildren() const
{
    return findObjects(Sel::depth(1).get());
}

std::string UiObject::getContentDescription() const
{
    return getAccessibleNode()->getDesc();
}

std::string UiObject::getApplicationPackage() const
{
    return getAccessibleNode()->getPkg();
}

std::string UiObject::getResourceName() const
{
    return getAccessibleNode()->getRes();
}

std::string UiObject::getText() const
{
    return getAccessibleNode()->getText();
}

void UiObject::setText(std::string &text)
{
    getAccessibleNode()->setValue(text);
}

bool UiObject::isCheckable() const
{
    return getAccessibleNode()->isCheckable();
}

bool UiObject::isChecked() const
{
    return getAccessibleNode()->isChecked();
}

bool UiObject::isClickable() const
{
    return getAccessibleNode()->isClickable();
}

bool UiObject::isEnabled() const
{
    return getAccessibleNode()->isEnabled();
}

bool UiObject::isFocusable() const
{
    return getAccessibleNode()->isFocusable();
}

bool UiObject::isFocused() const
{
    return getAccessibleNode()->isFocused();
}

bool UiObject::isLongClickable() const
{
    return getAccessibleNode()->isLongClickable();
}

bool UiObject::isScrollable() const
{
    return getAccessibleNode()->isScrollable();
}

bool UiObject::isSelectable() const
{
    return getAccessibleNode()->isSelectable();
}

bool UiObject::isSelected() const
{
    return getAccessibleNode()->isSelected();
}

bool UiObject::isVisible() const
{
    return getAccessibleNode()->isVisible();
}

void UiObject::refresh() const
{
    mNode->refresh();
}

const Rect<int> UiObject::getBoundingBox() const
{
    mNode->refresh();
    return mNode->getBoundingBox();
}

void UiObject::click() const
{
    LOG_SCOPE_F(INFO, "click on obj %p", this);
    mNode->refresh();
    const Rect<int> rect = mNode->getBoundingBox();
    std::cout << rect.mTopLeft.x << ", " << rect.mTopLeft.y << std::endl;
    const Point2D<int> midPoint = rect.midPoint();
    const_cast<UiDevice *>(mDevice)->click(midPoint.x, midPoint.y);
    // todo click implementation
}

void UiObject::longClick(const unsigned int intv) const
{
    LOG_SCOPE_F(INFO, "click on obj %p", this);
    mNode->refresh();
    const Rect<int> rect = mNode->getBoundingBox();
    std::cout << rect.mTopLeft.x << ", " << rect.mTopLeft.y << std::endl;
    const Point2D<int> midPoint = rect.midPoint();
    const_cast<UiDevice *>(mDevice)->click(midPoint.x, midPoint.y, intv);
}

const AccessibleNode *UiObject::getAccessibleNode() const
{
    if (mNode == nullptr) throw;

    // TODO : wait for animation and refresh current node
    // mDevice->waitForIdle();
    // mNode->refresh();

    return mNode;
}