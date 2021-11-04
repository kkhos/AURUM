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

#ifndef _UI_OBJECT_H_
#define _UI_OBJECT_H_

#include "config.h"

#include "AccessibleNode.h"
#include "ISearchable.h"
#include "UiSelector.h"
#include "Waiter.h"

#include <memory>
#include <vector>

namespace Aurum {

class UiDevice;

/**
 * @brief This class keep object's descendant tree.
 */
class Node : public std::enable_shared_from_this<Node> {
public:
    Node(std::shared_ptr<UiObject> node, std::vector<std::shared_ptr<Node>> children)
    :mNode{node}, mChildren{children}{}

    std::shared_ptr<UiObject> mNode;
    std::vector<std::shared_ptr<Node>> mChildren;
};

/**
 * @class UiObject
 *
 * @ingroup aurum
 *
 * @brief A UiObject is a representation of a actual object in view.
 *        As a class that abstracts the actual object, it has the information of the object
 *        such as object's properties, states, geometry information.
 *        also user can send and receive event via this class.
 */
class UiObject : public ISearchable , public std::enable_shared_from_this<UiObject> {
public:
    /**
     * @brief UiObject constructor with device, selector, node pointer.
     *
     * @since_tizen 6.5
     */
    UiObject(const std::shared_ptr<UiDevice> device, const std::shared_ptr<UiSelector> selector,
             const AccessibleNode *node);

    /**
     * @brief UiObject constructor with device, selector, node.
     *
     * @since_tizen 6.5
     */
    UiObject(const std::shared_ptr<UiDevice> device, const std::shared_ptr<UiSelector> selector,
             std::shared_ptr<AccessibleNode> node);

    /**
     * @brief UiObject constructor with object source.
     *
     * @since_tizen 6.5
     */
    UiObject(UiObject &&src);

    /**
     * @brief UiObject constructor. (default constructor)
     *
     * @since_tizen 6.5
     */
    UiObject();

    /**
     * @brief UiObject destructor.
     *
     * @since_tizen 6.5
     */
    virtual ~UiObject();

    /**
     * @brief Gets selector.
     *
     * @return UiSelector pointer
     *
     * @since_tizen 6.5
     */
    std::shared_ptr<UiSelector> getSelector();

    /**
     * @brief Checks that there is object that satisfied with the selector condition in the object tree.
     *
     * @param[in] selector @UiSelector
     *
     * @return true if object exist, otherwise false
     *
     * @since_tizen 6.5
     */
    bool hasObject(const std::shared_ptr<UiSelector> selector) const override;

    /**
     * @brief Finds that object that satisfied with the selector condition in the object tree.
     *
     * @param[in] selector @UiSelector
     *
     * @return Found UiObject pointer
     *
     * @since_tizen 6.5
     */
    std::shared_ptr<UiObject> findObject(
        const std::shared_ptr<UiSelector> selector) const override;

    /**
     * @brief Finds that objects that satisfied with the selector condition in the object tree.
     *
     * @param[in] selector @UiSelector
     *
     * @return the list of found UiObject pointer vector
     *
     * @since_tizen 6.5
     */
    std::vector<std::shared_ptr<UiObject>> findObjects(
        const std::shared_ptr<UiSelector> selector) const override;

    /**
     * TODO
     */
    bool waitFor(
        const std::function<bool(const ISearchable *)> condition) const;

    /**
     * TODO
     */
    std::shared_ptr<UiObject> waitFor(
        const std::function<std::shared_ptr<UiObject>(const ISearchable *)>
            condition) const;

    /**
     * TODO
     */
    bool waitFor(const std::function<bool(const UiObject *)> condition) const;

public:
    /**
     * @brief Gets object's parent.
     *
     * @return UiObject pointer
     *
     * @since_tizen 6.5
     */
    UiObject *getParent() const;

    /**
     * @brief Gets object's child count.
     *
     * @return number of child
     *
     * @since_tizen 6.5
     */
    int getChildCount() const;

    /**
     * @brief Gets object's children.
     *
     * @return UiObject pointer vector
     *
     * @since_tizen 6.5
     */
    std::vector<std::shared_ptr<UiObject>> getChildren() const;

    /**
     * @brief Gets object's child at specific index.
     *
     * @return UiObject pointer
     *
     * @since_tizen 6.5
     */
    std::shared_ptr<UiObject> getChildAt(int index) const;

    /**
     * @brief Gets object's Descendant tree .
     *
     * @return Node pointer vector
     *
     * @since_tizen 6.5
     */
    std::shared_ptr<Node> getDescendant();

    /**
     * @brief Gets object's package name.
     *
     * @return string
     *
     * @since_tizen 6.5
     */
    std::string getApplicationPackage() const;

    /**
     * @brief Gets object's Id.
     *
     * @return string
     *
     * @since_tizen 6.5
     */
    std::string getId() const;

    /**
     * @brief Gets object's automation Id.
     *
     * @return string
     *
     * @since_tizen 6.5
     */
    std::string getAutomationId() const;

    /**
     * @brief Gets object's type.
     *
     * @return string
     *
     * @since_tizen 6.5
     */
    std::string getElementType() const;

    /**
     * @brief Gets object's style.
     *
     * @return string
     *
     * @since_tizen 6.5
     */
    std::string getElementStyle() const;

    /**
     * @brief Gets object's text.
     *
     * @return string
     *
     * @since_tizen 6.5
     */
    std::string getText() const;

    /**
     * @brief Gets object's role.
     *
     * @return string
     *
     * @since_tizen 6.5
     */
    std::string getRole() const;

    /**
     * @brief Sets object's text.
     *
     * @param[in] text string
     *
     * @return string
     *
     * @since_tizen 6.5
     */
    void setText(std::string text);

    /**
     * @brief Gets object's geometry of the screen.
     *
     * @return @Rect
     *
     * @since_tizen 6.5
     */
    const Rect<int> getScreenBoundingBox() const;

    /**
     * @brief Gets object's geometry of the window.
     *
     * @return @Rect
     *
     * @since_tizen 6.5
     */
    const Rect<int> getWindowBoundingBox() const;

    /**
     * @brief Gets object's checkable property.
     *
     * @return true if checkable else false
     *
     * @since_tizen 6.5
     */
    bool isCheckable() const;

    /**
     * @brief Gets object's checked property.
     *
     * @return true if checked else false
     *
     * @since_tizen 6.5
     */
    bool isChecked() const;

    /**
     * @brief Gets object's clickable property.
     *
     * @return true if clickable else false
     *
     * @since_tizen 6.5
     */
    bool isClickable() const;

    /**
     * @brief Gets object's enabled property. (to get object enabled. disabled state)
     *
     * @return true if enabled else false
     *
     * @since_tizen 6.5
     */
    bool isEnabled() const;

    /**
     * @brief Gets object's focusable property.
     *
     * @return true if focusable else false
     *
     * @since_tizen 6.5
     */
    bool isFocusable() const;

    /**
     * @brief Gets object's focused property.
     *
     * @return true if focused else false
     *
     * @since_tizen 6.5
     */
    bool isFocused() const;

    /**
     * @brief Gets object's longclickable property.
     *
     * @return true if longclickable else false
     *
     * @since_tizen 6.5
     */
    bool isLongClickable() const;

    /**
     * @brief Gets object's scrollable property.
     *
     * @return true if scrollable else false
     *
     * @since_tizen 6.5
     */
    bool isScrollable() const;

    /**
     * @brief Gets object's selectable property.
     *
     * @return true if selectable else false
     *
     * @since_tizen 6.5
     */
    bool isSelectable() const;

    /**
     * @brief Gets object's selected property.
     *
     * @return true if selected else false
     *
     * @since_tizen 6.5
     */
    bool isSelected() const;

    /**
     * @brief Gets object's visible property.
     *
     * @return true if visible else false
     *
     * @since_tizen 6.5
     */
    bool isVisible() const;

    /**
     * @brief Gets object's showing property.
     *
     * @return true if showing else false
     *
     * @since_tizen 6.5
     */
    bool isShowing() const;

    /**
     * @brief Gets object's active property.
     *
     * @return true if active else false
     *
     * @since_tizen 6.5
     */
    bool isActive() const;

    /**
     * @brief Performs a click action on object.
     *
     * @since_tizen 6.5
     */
    void click() const;

    /**
     * @brief Performs a long click action on object.
     *
     * @param[in] durationMs total time to maintain down action (default = 500ms)
     *
     * @since_tizen 6.5
     */
    void longClick(const unsigned int durationMs = LOGNCLICK_INTERVAL) const;

    /**
     * TODO
     */
    bool DoAtspiActivate() const;

    /**
     * @brief Updates object's role name information from atspi server.
     *
     * @since_tizen 6.5
     */
    void updateRoleName() const;

    /**
     * @brief Updates object's Id information from atspi server.
     *
     * @since_tizen 6.5
     */
    void updateUniqueId() const;

    /**
     * @brief Updates object's name information from atspi server.
     *
     * @since_tizen 6.5
     */
    void updateName() const;

    /**
     * @brief Updates object's application name information from atspi server.
     *
     * @since_tizen 6.5
     */
    void updateApplication() const;

    /**
     * @brief Updates object's attributes information from atspi server.
     *
     * @since_tizen 6.5
     */
    void updateAttributes() const;

    /**
     * @brief Updates object's states information from atspi server.
     *
     * @since_tizen 6.5
     */
    void updateStates() const;

    /**
     * @brief Updates object's geometry information from atspi server.
     *
     * @since_tizen 6.5
     */
    void updateExtents() const;

    /**
     * @brief Updates object's information from atspi server.
     *
     * @since_tizen 6.5
     */
    void refresh() const;

    /**
     * @brief Checks object is valid or not.
     *
     * @return true if valid else false
     *
     * @since_tizen 6.5
     */
    bool isValid() const;

    /**
     * @brief Gets object's AccessibleNode class
     *
     * @return AccessibleNode pointer
     *
     * @since_tizen 6.5
     */
    std::shared_ptr<AccessibleNode> getAccessibleNode() const;

private:
    std::shared_ptr<UiDevice> mDevice;
    std::shared_ptr<UiSelector>  mSelector;
    std::shared_ptr<AccessibleNode> mNode;
    const Waiter *mWaiter;
    static const unsigned int LOGNCLICK_INTERVAL = 500;
};

}

#endif
