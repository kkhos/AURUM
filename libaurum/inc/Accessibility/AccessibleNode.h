/*
 * Copyright (c) 2024 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef _ACCESSIBLE_NODE_H_
#define _ACCESSIBLE_NODE_H_

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <mutex>

#include "IEventConsumer.h"
#include "Rect.h"
#include "UiSelector.h"
#include "config.h"

namespace Aurum {

/**
 * @brief AccessibleNodeInterface enum class
 *
 * @since_tizen 6.5
 */
enum class AccessibleNodeInterface {
    ACTION          = 0x0001,
    COLLECTION      = 0X0002,
    COMPONENT       = 0X0004,
    DOCUMENT        = 0X0008,

    EDITABLETEXT    = 0X0010,
    HYPERTEXT       = 0X0020,
    IMAGE           = 0X0040,
    SELECTION       = 0X0080,

    TEXT            = 0X0100,
    VALUE           = 0X0200,
    ACCESSIBLE      = 0X0400,
    TABLE           = 0X0800,

    TABLECELL       = 0X1000,
};

/**
 * @brief NodeFeatureProperties enum class
 *
 * @since_tizen 6.5
 */
enum class NodeFeatureProperties {
    CHECKABLE       = 0x0001,
    CHECKED         = 0X0002,
    CLICKABLE       = 0X0004,
    ENABLED         = 0X0008,

    FOCUSABLE       = 0X0010,
    FOCUSED         = 0X0020,
    LONGCLICKABLE   = 0X0040,
    SCROLLABLE      = 0X0080,

    SELECTABLE      = 0X0100,
    SELECTED        = 0X0200,
    VISIBLE         = 0X0400,
    SHOWING         = 0X0800,
    ACTIVE          = 0X1000,
    HIGHLIGHTABLE   = 0X2000,
    HIGHLIGHTED     = 0X4000,
    INVALID         = 0X8000,
};

/**
 * @brief AccessibleNode Class that provides the abstracted object information to uses.
 *
 * @since_tizen 6.5
 */
class AccessibleNode : public std::enable_shared_from_this<AccessibleNode>, public IEventConsumer {
public:
    /**
     * @brief AccessibleNode constructor.
     *
     * @since_tizen 6.5
     */
    AccessibleNode();

    /**
     * @brief AccessibleNode destructor.
     *
     * @since_tizen 6.5
     */
    virtual ~AccessibleNode();

    /**
     * @brief Gets Node information as string.
     *
     * @return string
     *
     * @since_tizen 6.5
     */
    std::string description();

public:
    /**
     * @copydoc UiObject::getChildCount()
     */
    virtual int getChildCount() const = 0;

    /**
     * @copydoc UiObject::getChildAt()
     */
    virtual std::shared_ptr<AccessibleNode> getChildAt(int index) const = 0;

    /**
     * @copydoc UiObject::getChildren()
     */
    virtual std::vector<std::shared_ptr<AccessibleNode>> getChildren() const = 0;

    /**
     * @copydoc UiObject::getParent()
     */
    virtual std::shared_ptr<AccessibleNode> getParent() const = 0;

    /**
     * @copydoc UiObject::getMatches()
     */
    virtual std::vector<std::shared_ptr<AccessibleNode>> getMatches(const std::shared_ptr<UiSelector> selector, const bool ealryReturn) const = 0;

    /**
     * @copydoc UiObject::getMatchesInMatches()
     */
    virtual std::vector<std::shared_ptr<AccessibleNode>> getMatchesInMatches(const std::shared_ptr<UiSelector> firstSelector, const std::shared_ptr<UiSelector> secondSelector, const bool ealryReturn) const = 0;

    /**
     * @copydoc UiObject::next()
     */
    virtual std::shared_ptr<AccessibleNode> next() const = 0;

    /**
     * @copydoc UiObject::prev()
     */
    virtual std::shared_ptr<AccessibleNode> prev() const = 0;

    /**
     * @copydoc UiObject::first()
     */
    virtual std::shared_ptr<AccessibleNode> first() const = 0;

    /**
     * @copydoc UiObject::last()
     */
    virtual std::shared_ptr<AccessibleNode> last() const = 0;

    /**
     * @brief Called by @AccessibleWatcher::notifyAll.
     *        Changes Node property If it's @EventType, @ObjectEventType are matches.
     *
     * @param[in] type @EventType
     * @param[in] type2 @ObjectEventType
     * @param[in] src Atspi Node ptr
     *
     * @since_tizen 6.5
     */
    void notify(EventType type, void *src) override;

    /**
     * @brief Changes Node state to invalidate.
     *
     * @since_tizen 6.5
     */
    void invalidate();

public:
    /**
     * @copydoc UiObject::getId()
     */
    std::string getId() const;

    /**
     * @copydoc UiObject::getType()
     */
    std::string getType() const;

    /**
     * @copydoc UiObject::getScreenBoundingBox()
     */
    const Rect<int> getScreenBoundingBox() const;

    /**
     * @copydoc UiObject::getOcrText()
     */
    std::string getOcrText() const;

    /**
     * @copydoc UiObject::getWindowAngle()
     */
    int getWindowAngle() const;

    /**
     * @copydoc UiObject::getTargetAngle()
     */
    int getTargetAngle() const;

    /**
     * @copydoc UiObject::isFocusable()
     */
    bool isFocusable() const;

    /**
     * @copydoc UiObject::isFocused()
     */
    bool isFocused() const;

    /**
     * @copydoc UiObject::isClickable()
     */
    bool isClickable() const;

    /**
     * @copydoc UiObject::isActive()
     */
    bool isActive() const;

    /**
     * @copydoc UiObject::isShowing()
     */
    bool isShowing() const;

public:
    /**
     * @copydoc UiObject::getText()
     */
    std::string getText() const;

    /**
     * @copydoc UiObject::setText()
     */
    bool setText(std::string text);

    /**
     * @copydoc UiObject::setOcrText()
     */
    void setOcrText(std::string text);

    /**
     * @brief Sets Node's angle of window.
     *
     * @param[in] int window angle
     *
     * @since_tizen 7.5
     */
    void setWindowAngle(int windowAngle);

    /**
     * @brief Sets Node's angle of target device.
     *
     * @param[in] int target angle
     *
     * @since_tizen 7.5
     */
    void setTargetAngle(int targetAngle);

    /**
     * @copydoc UiObject::getPkg()
     */
    std::string getPkg() const;

    /**
     * @copydoc UiObject::getAutomationId()
     */
    std::string getAutomationId() const;

    /**
     * @copydoc UiObject::getRole()
     */
    std::string getRole() const;

    /**
     * @copydoc UiObject::getStyle()
     */
    std::string getStyle() const;

    /**
     * @copydoc UiObject::getToolkitName()
     */
    std::string getToolkitName() const;

    /**
     * @copydoc UiObject::getWindowBoundingBox()
     */
    const Rect<int> getWindowBoundingBox() const;

    /**
     * @copydoc UiObject::getXPath()
     */
    std::string getXPath() const;

    /**
     * @copydoc UiObject::getMinValue()
     *
     */
    double getMinValue() const;

    /**
     * @copydoc UiObject::getMaxValue()
     *
     */
    double getMaxValue() const;

    /**
     * @copydoc UiObject::getValue()
     *
     */
    double getValue() const;

    /**
     * @copydoc UiObject::getPid()
     *
     */
    int getPid() const;

    /**
     * @copydoc UiObject::getIncrement()
     *
     */
    double getIncrement() const;

    /**
     * @copydoc UiObject::getTextMinBoundingRect()
     *
     */
    Rect<int> getTextMinBoundingRect() const;

    /**
     * @copydoc UiObject::getInterface()
     */
    std::string getInterface() const;

    /**
     * @copydoc UiObject::getDescription()
     */
    std::string getDescription() const;

    /**
     * @copydoc UiObject::getImgSrc()
     */
    std::string getImgSrc() const;

    /**
     * @copydoc UiObject::isCheckable()
     */
    bool isCheckable() const;

    /**
     * @copydoc UiObject::isChecked()
     */
    bool isChecked() const;

    /**
     * @copydoc UiObject::isEnabled()
     */
    bool isEnabled() const;

    /**
     * @copydoc UiObject::isLongClickable()
     */
    bool isLongClickable() const;

    /**
     * @copydoc UiObject::isScrollable()
     */
    bool isScrollable() const;

    /**
     * @copydoc UiObject::isSelectable()
     */
    bool isSelectable() const;

    /**
     * @copydoc UiObject::isSelected()
     */
    bool isSelected() const;

    /**
     * @copydoc UiObject::isVisible()
     */
    bool isVisible() const;

    /**
     * @copydoc UiObject::isHighlightable()
     */
    bool isHighlightable() const;

    /**
     * @copydoc UiObject::isHighlighted()
     */
    bool isHighlighted() const;

public:
    /**
     * @brief Print Node information.
     *
     * @param[in] int depth
     *
     * @since_tizen 6.5
     */
    void print(int);

    /**
     * @brief Print Node information.
     *
     * @param[in] int depth
     * @param[in] int maxdepth
     *
     * @since_tizen 6.5
     */
    void print(int, int);

    /**
     * @brief Gets matched Atspi node ptr.
     *
     * @return Atspi node ptr
     *
     * @since_tizen 6.5
     */
    virtual void* getRawHandler(void) const = 0;

    /**
     * @copydoc UiObject::updateRoleName()
     */
    virtual void updateRoleName() = 0;

    /**
     * @copydoc UiObject::updateUniqueId()
     */
    virtual void updateUniqueId() = 0;

    /**
     * @copydoc UiObject::updateName()
     */
    virtual void updateName() = 0;

    /**
     * @copydoc UiObject::updateApplication()
     */
    virtual void updateApplication() = 0;

    /**
     * @copydoc UiObject::updateAttributes()
     */
    virtual void updateAttributes() = 0;

    /**
     * @copydoc UiObject::updateStates()
     */
    virtual void updateStates() = 0;

    /**
     * @copydoc UiObject::updateExtents()
     */
    virtual void updateExtents() = 0;

    /**
     * @copydoc UiObject::updateXPath()
     */
    virtual void updateXPath() = 0;

    /**
     * @copydoc UiObject::updateToolkitName()
     */
    virtual void updateToolkitName() = 0;

    /**
     * @copydoc UiObject::updateValue()
     */
    virtual void updateValue() = 0;

    /**
     * @copydoc UiObject::updatePid()
     */
    virtual void updatePid() = 0;

    /**
     * @copydoc UiObject::setFocus()
     */
    virtual bool setFocus() = 0;

    /**
     * @copydoc UiObject::moveTo()
     */
    virtual bool moveTo() = 0;

    /**
     * @copydoc UIObject::updateTextMinBoundingRect()
     */
    virtual void updateTextMinBoundingRect() = 0;

    /**
     * @copydoc UiObject::updateInterface()
     */
    virtual void updateInterface() = 0;

    /**
     * @brief Updates Node information from atspi server.
     *
     * @since_tizen 6.5
     */
    virtual void refresh(bool updateAll = true) = 0;

    /**
     * @brief Gets available atspi action name.
     *
     * @return string vector
     *
     * @since_tizen 6.5
     */
    virtual std::vector<std::string> getActions() const = 0;

    /**
     * @brief Do atspi action.
     *
     * @param[in] action name of action
     *
     * @return true if success, else false
     *
     * @since_tizen 6.5
     */
    virtual bool doAction(std::string action) = 0;

    /**
     * @brief Sets Node's value.
     *
     * @param[in] text string
	 *
	 * @return true if success, else false
     *
     * @since_tizen 6.5
     */
    virtual bool setValue(std::string text) = 0;

    /**
     * @brief Sets Node's value.
     *
     * @param[in] value double
     *
     * @return true if success, else false
     *
     * @since_tizen 7.0
     */
    virtual bool setValue(double value) = 0;

    /**
     * @brief Check object valid or not.
     *
     * @return true if valid, else false
     *
     * @since_tizen 6.5
     */
    virtual bool isValid() const;

    /**
     * @copydoc UIObject::setIncludeHidden()
     */
    virtual void setIncludeHidden(bool enabled) const = 0;

    /**
     * @copydoc UIObject::getIncludeHidden()
     */
    virtual bool getIncludeHidden() const = 0;

public:
    /**
     * @brief Check Node support given interface or not.
     *
     * @param[in] thisIface @AccessibleNodeInterface
     *
     * @return true if supporting, else false
     *
     * @since_tizen 6.5
     */
    bool isSupporting(AccessibleNodeInterface thisIface) const;

    /**
     * @brief Check Node has given property or not.
     *
     * @param[in] prop @NodeFeatureProperties
     *
     * @return true if has, else false
     *
     * @since_tizen 6.5
     */
    bool hasFeatureProperty(NodeFeatureProperties prop) const;

    /**
     * @brief Sets Node's property.
     *
     * @param[in] prop @NodeFeatureProperties
     * @param[in] has Node has given property or not
     *
     * @since_tizen 6.5
     */
    void setFeatureProperty(NodeFeatureProperties prop, bool has);

    /**
     * @brief Resets all the property value on Node.
     *
     * @since_tizen 6.5
     */
    void resetFeatureProperty();

protected:
    std::string mText;
    std::string mOcrText;
    std::string mPkg;
    std::string mRole;
    std::string mId;
    std::string mAutomationId;
    std::string mType;
    std::string mStyle;
    std::string mXPath;
    std::string mToolkitName;
    std::string mInterface;
    std::string mDescription;
    std::string mImgSrc;
    Rect<int> mScreenBoundingBox;
    Rect<int> mWindowBoundingBox;
    Rect<int> mTextMinBoundingRect;
    int mSupportingIfaces;
    int mFeatureProperty;
    int mPid;
    int mWindowAngle;
    int mTargetAngle;
    double mMinValue;
    double mMaxValue;
    double mValue;
    double mIncrement;

private:
    bool mValid;
    mutable std::mutex mLock;
};

}

#endif
