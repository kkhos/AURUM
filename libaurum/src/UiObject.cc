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

#include <iostream>
#include <utility>

#include <chrono>
#include <thread>

using namespace Aurum;

UiObject::UiObject() : UiObject(nullptr, nullptr, nullptr) {}

UiObject::~UiObject()
{
    if (mWaiter) delete mWaiter;
}

UiObject::UiObject(const std::shared_ptr<UiDevice> device, const std::shared_ptr<UiSelector> selector,
                   const AccessibleNode *node)
    : mDevice(device),
      mSelector(selector),
      mNode(std::shared_ptr<AccessibleNode>(const_cast<AccessibleNode *>(node))),
      mWaiter(new Waiter{this, this})
{
}

UiObject::UiObject(const std::shared_ptr<UiDevice> device, const std::shared_ptr<UiSelector> selector,
                   std::shared_ptr<AccessibleNode> node)
    : mDevice(device),
      mSelector(selector),
      mNode(std::move(node)),
      mWaiter(new Waiter{this, this})
{
}

UiObject::UiObject(UiObject &&src)
    : mDevice(src.mDevice),
      mSelector(std::move(src.mSelector)),
      mNode(std::move(src.mNode)),
      mWaiter{src.mWaiter}
{
    src.mDevice = nullptr;
    src.mSelector = nullptr;
    src.mNode = nullptr;
    src.mWaiter = nullptr;
}

std::shared_ptr<UiSelector> UiObject::getSelector()
{
    return this->mSelector;
}

bool UiObject::hasObject(const std::shared_ptr<UiSelector> selector) const
{
    std::shared_ptr<AccessibleNode> node =
        Comparer::findObject(mDevice, selector, mNode);
    if (node != nullptr) {
        // todo : what is this node.recycle()
        return true;
    }
    return false;
}

std::shared_ptr<UiObject> UiObject::findObject(const std::shared_ptr<UiSelector> selector) const
{
    std::shared_ptr<AccessibleNode> node =
        Comparer::findObject(mDevice, selector, mNode);
    if (node)
        return std::make_shared<UiObject>(mDevice, selector, std::move(node));
    else
        return std::shared_ptr<UiObject>{nullptr};
}

std::vector<std::shared_ptr<UiObject>> UiObject::findObjects(
    const std::shared_ptr<UiSelector> selector) const
{
    std::vector<std::shared_ptr<UiObject>> result{};

    std::vector<std::shared_ptr<AccessibleNode>> nodes{};
    Comparer::findObjects(nodes, mDevice, selector, mNode);
    for ( auto& node : nodes) {
        if (!node) {
            LOGI("Skipped! (node == nullptr)");
            continue;
        }
        result.push_back(std::make_shared<UiObject>(mDevice, selector, std::move(node)));
    }
    return result;
}

std::vector<std::shared_ptr<UiObject>> UiObject::getMatches(
    const std::shared_ptr<UiSelector> selector, const bool earlyReturn) const
{
    std::vector<std::shared_ptr<UiObject>> result{};

    auto nodes = mNode->getMatches(selector, earlyReturn);
    for (auto &node : nodes) {
        result.push_back(std::make_shared<UiObject>(mDevice, selector, std::move(node)));
    }
    return result;
}

std::vector<std::shared_ptr<UiObject>> UiObject::getMatchesInMatches(
    const std::shared_ptr<UiSelector> firstSelector, const std::shared_ptr<UiSelector> secondSelector, const bool earlyReturn) const
{
    std::vector<std::shared_ptr<UiObject>> result{};

    auto nodes = mNode->getMatchesInMatches(firstSelector, secondSelector, earlyReturn);
    for (auto &node : nodes) {
        result.push_back(std::make_shared<UiObject>(mDevice, nullptr, std::move(node)));
    }
    return result;
}

bool UiObject::waitFor(
    const std::function<bool(const ISearchable *)> condition) const
{
    return mWaiter->waitFor(condition);
}

std::shared_ptr<UiObject> UiObject::waitFor(
    const std::function<std::shared_ptr<UiObject>(const ISearchable *)>
        condition) const
{
    return mWaiter->waitFor(condition);
}

bool UiObject::waitFor(
    const std::function<bool(const UiObject *)> condition) const
{
    return mWaiter->waitFor(condition);
}

std::shared_ptr<UiObject> UiObject::getParent() const
{
    std::shared_ptr<AccessibleNode> node = mNode->getParent();
    if (!node) return nullptr;
    return std::make_shared<UiObject>(mDevice, mSelector, node);
}

int UiObject::getChildCount() const
{
    return mNode->getChildCount();
}

std::shared_ptr<UiObject> UiObject::getChildAt(int index) const {
    auto childNode = mNode->getChildAt(index);
    if (childNode) {
        return std::make_shared<UiObject>(mDevice, mSelector, childNode);
    }
    return nullptr;
}

std::vector<std::shared_ptr<UiObject>> UiObject::getChildren() const
{
    std::vector<std::shared_ptr<UiObject>> ret{};

    auto children = mNode->getChildren();
    for (auto &child : children) {
        ret.push_back(std::make_shared<UiObject>(mDevice, mSelector, child));
    }

    return ret;
}

std::shared_ptr<Node> UiObject::getDescendant()
{
    std::vector<std::shared_ptr<Node>> nodeChildren{};

    auto children = getChildren();
    for (auto &&child : children) {
        nodeChildren.push_back(child->getDescendant());
    }
    return std::make_shared<Node>(shared_from_this(), nodeChildren);
}

std::string UiObject::getApplicationPackage() const
{
    return mNode->getPkg();
}

std::string UiObject::getId() const
{
    return mNode->getId();
}

std::string UiObject::getAutomationId() const
{
    return mNode->getAutomationId();
}

std::string UiObject::getType() const
{
    return mNode->getType();
}

std::string UiObject::getElementStyle() const
{
    return mNode->getStyle();
}

std::string UiObject::getText() const
{
    return mNode->getText();
}

std::string UiObject::getRole() const
{
    return mNode->getRole();
}

std::string UiObject::getXPath() const
{
    return mNode->getXPath();
}

const double UiObject::getMinValue() const
{
    return mNode->getMinValue();
}

const double UiObject::getMaxValue() const
{
    return mNode->getMaxValue();
}

const double UiObject::getValue() const
{
    return mNode->getValue();
}

const double UiObject::getIncrement() const
{
    return mNode->getIncrement();
}

const Rect<int> UiObject::getTextMinBoundingRect() const
{
    return mNode->getTextMinBoundingRect();
}

std::string UiObject::getInterface() const
{
    return mNode->getInterface();
}

std::string UiObject::getDescription() const
{
    return mNode->getDescription();
}

std::string UiObject::getImgSrc() const
{
    return mNode->getImgSrc();
}

bool UiObject::setValue(double value)
{
    return mNode->setValue(value);
}

bool UiObject::setText(std::string text)
{
    return mNode->setValue(text);
}

std::string UiObject::getOcrText() const
{
    return mNode->getOcrText();
}

int UiObject::getWindowAngle() const
{
    return mNode->getWindowAngle();
}

int UiObject::getTargetAngle() const
{
    return mNode->getTargetAngle();
}

std::string UiObject::getToolkitName() const
{
    mNode->updateToolkitName();
    return mNode->getToolkitName();
}

void UiObject::setOcrText(std::string text)
{
    mNode->setOcrText(text);
}

bool UiObject::isCheckable() const
{
    return mNode->isCheckable();
}

bool UiObject::isChecked() const
{
    return mNode->isChecked();
}

bool UiObject::isClickable() const
{
    return mNode->isClickable();
}

bool UiObject::isEnabled() const
{
    return mNode->isEnabled();
}

bool UiObject::isFocusable() const
{
    return mNode->isFocusable();
}

bool UiObject::isFocused() const
{
    return mNode->isFocused();
}

bool UiObject::isLongClickable() const
{
    return mNode->isLongClickable();
}

bool UiObject::isScrollable() const
{
    return mNode->isScrollable();
}

bool UiObject::isSelectable() const
{
    return mNode->isSelectable();
}

bool UiObject::isSelected() const
{
    return mNode->isSelected();
}

bool UiObject::isVisible() const
{
    return mNode->isVisible();
}

bool UiObject::isShowing() const
{
    return mNode->isShowing();
}

bool UiObject::isActive() const
{
    return mNode->isActive();
}

bool UiObject::isHighlightable() const
{
    return mNode->isHighlightable();
}

void UiObject::refresh() const
{
    mNode->refresh();
}

void UiObject::updateRoleName() const
{
    mNode->updateRoleName();
}

void UiObject::updateUniqueId() const
{
    mNode->updateUniqueId();
}

void UiObject::updateName() const
{
    mNode->updateName();
}

void UiObject::updateApplication() const
{
    mNode->updateApplication();
}

void UiObject::updateAttributes() const
{
    mNode->updateAttributes();
}

void UiObject::updateStates() const
{
    mNode->updateStates();
}

void UiObject::updateExtents() const
{
    mNode->updateExtents();
}

void UiObject::updateXPath() const
{
    mNode->updateXPath();
}

void UiObject::updateValue() const
{
    mNode->updateValue();
}

void UiObject::updatePid() const
{
    mNode->updatePid();
}

void UiObject::updateToolkitName() const
{
    mNode->updateToolkitName();
}

void UiObject::updateTextMinBoundingRect() const
{
    mNode->updateTextMinBoundingRect();
}

bool UiObject::setFocus() const
{
    return mNode->setFocus();
}

bool UiObject::moveTo() const
{
    return mNode->moveTo();
}

bool UiObject::isValid() const
{
    return mNode->isValid();
}

const Rect<int> UiObject::getScreenBoundingBox() const
{
    return mNode->getScreenBoundingBox();
}

const Rect<int> UiObject::getWindowBoundingBox() const
{
    return mNode->getWindowBoundingBox();
}

const Point2D<int> getMidPoint(std::shared_ptr<Aurum::AccessibleNode> node)
{
    node->updateExtents();
    const Rect<int> rect = node->getScreenBoundingBox();
    return rect.midPoint();
}

void UiObject::click() const
{
    const Point2D<int> midPoint = getMidPoint(mNode);
    mDevice->click(midPoint.x, midPoint.y);
}

void UiObject::longClick(const unsigned int durationMs) const
{
    const Point2D<int> midPoint = getMidPoint(mNode);
    mDevice->click(midPoint.x, midPoint.y, durationMs);
}

bool UiObject::DoAtspiActivate() const
{
    return mNode->doAction("activate");
}


std::shared_ptr<AccessibleNode> UiObject::getAccessibleNode() const
{
    if (mNode == nullptr) throw;
    // TODO : wait for animation and refresh current node
    // mDevice->waitForIdle();
    return mNode;
}

std::shared_ptr<UiObject> UiObject::next() const {
    auto nextNode = mNode->next();
    if (nextNode) {
        return std::make_shared<UiObject>(mDevice, mSelector, nextNode);
    }
    return nullptr;
}

std::shared_ptr<UiObject> UiObject::prev() const {
    auto prevNode = mNode->prev();
    if (prevNode) {
        return std::make_shared<UiObject>(mDevice, mSelector, prevNode);
    }
    return nullptr;
}

std::shared_ptr<UiObject> UiObject::first() const {
    auto firstNode = mNode->first();
    if (firstNode) {
        return std::make_shared<UiObject>(mDevice, mSelector, firstNode);
    }
    return nullptr;
}

std::shared_ptr<UiObject> UiObject::last() const {
    auto lastNode = mNode->last();
    if (lastNode) {
        return std::make_shared<UiObject>(mDevice, mSelector, lastNode);
    }
    return nullptr;
}

void UiObject::setIncludeHidden(bool enabled) const {
    mNode->setIncludeHidden(enabled);
}

bool UiObject::getIncludeHidden() const {
    return mNode->getIncludeHidden();
}