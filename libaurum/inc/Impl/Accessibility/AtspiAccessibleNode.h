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

#ifndef _ATSPI_ACCESSIBLE_NODE_H_
#define _ATSPI_ACCESSIBLE_NODE_H_

#include "AccessibleNode.h"
#include <atspi/atspi.h>

using namespace Aurum;

namespace AurumInternal {

/**
 * @brief AtspiAccessibleNode is inherited from AccessibleNode class.
 *        It manages object informations which from atspi server.
 *
 * @since_tizen 6.5
 */
class AtspiAccessibleNode : public AccessibleNode {
public:
    /**
     * @brief AtspiAccessibleNode constructor.
     *
     * @param[in] node AtspiAccessible ptr
     *
     * @since_tizen 6.5
     */
    AtspiAccessibleNode(AtspiAccessible *node);

    /**
     * @brief AtspiAccessibleNode destructor.
     *
     * @since_tizen 6.5
     */
   ~AtspiAccessibleNode() override;

public:
    /**
     * @copydoc UiObject::getChildCount()
     */
    int getChildCount() const override;

    /**
     * @copydoc UiObject::getChildAt()
     */
    std::shared_ptr<AccessibleNode> getChildAt(int index) const override;

    /**
     * @copydoc UiObject::getChildren()
     */
    std::vector<std::shared_ptr<AccessibleNode>> getChildren() const override;

    /**
     * @copydoc UiObject::getParent()
     */
    std::shared_ptr<AccessibleNode> getParent() const override;

    /**
     * @copydoc UiObject::getMatches()
     */
    std::vector<std::shared_ptr<AccessibleNode>> getMatches(const std::shared_ptr<UiSelector> selector, const bool ealryReturn) const override;

    /**
     * @copydoc UiObject::getMatchesInMatches()
     */
    std::vector<std::shared_ptr<AccessibleNode>> getMatchesInMatches(const std::shared_ptr<UiSelector> firstSelector, const std::shared_ptr<UiSelector> secondSelector, const bool ealryReturn) const override;

    /**
     * @copydoc AccessibleNode::dumpTree()
     */
    std::string dumpTree() const override;

    /**
     * @copydoc UiObject::next()
     */
    std::shared_ptr<AccessibleNode> next() const override;

        /**
     * @copydoc UiObject::prev()
     */
    std::shared_ptr<AccessibleNode> prev() const override;

        /**
     * @copydoc UiObject::first()
     */
    std::shared_ptr<AccessibleNode> first() const override;

        /**
     * @copydoc UiObject::last()
     */
    std::shared_ptr<AccessibleNode> last() const override;

    /**
     * @copydoc AccessibleNode::refAccessibleNode()
     */
    std::shared_ptr<AccessibleNode> refAccessibleNode(const std::string &appName, const std::string &path ) const override;

    /**
     * @copydoc AccessibleNode::getStringProperty()
     */
    std::string getStringProperty(const std::string &property) const override;

    /**
     * @copydoc AccessibleNode::isValid()
     */
    bool isValid() const override;

public:
    /**
     * @copydoc AccessibleNode::getRawHandler()
     */
    void* getRawHandler(void) const override;

    /**
     * @copydoc UiObject::updateRoleName()
     */
    void updateRoleName() override;

    /**
     * @copydoc UiObject::updateUniqueId()
     */
    void updateUniqueId() override;

    /**
     * @copydoc UiObject::updateName()
     */
    void updateName() override;

    /**
     * @copydoc UiObject::updateApplication()
     */
    void updateApplication() override;

    /**
     * @copydoc UiObject::updateAttributes()
     */
    void updateAttributes() override;

    /**
     * @copydoc UiObject::updateStates()
     */
    void updateStates() override;

    /**
     * @copydoc UiObject::updateExtents()
     */
    void updateExtents() override;

    /**
     * @copydoc UiObject::updateXPath()
     */
    void updateXPath() override;

    /**
     * @copydoc UiObject::updateToolkitName()
     */
    void updateToolkitName() override;

    /**
     * @copydoc UiObject::updateValue()
     */
    void updateValue() override;

    /**
     * @copydoc UiObject::updatePid()
     */
    void updatePid() override;

    /**
     * @copydoc UiObject::updateTextMinBoundingRect()
     */
    void updateTextMinBoundingRect() override;

    /**
     * @copydoc UiObject::updateInterface()
     */
    void updateInterface() override;

    /**
     * @copydoc UiObject::setFocus()
     */
    bool setFocus() override;

    /**
     * @copydoc UiObject::moveTo()
     */
    bool moveTo() override;

    /**
     * @copydoc AccessibleNode::refresh()
     */
    void refresh(bool updateAll = true) override;

    /**
     * @copydoc AccessibleNode::refresh()
     */
    void refresh(const std::string &text, const std::string &role, const std::string &type,
                 const std::string &automationId, const std::string &description, const std::string &imgSrc,
                 double value, double minValue, double maxValue, double increment, const Rect<int> &extents,
                 const std::string &valueText) override;

    /**
     * @copydoc AccessibleNode::getActions()
     */
    std::vector<std::string> getActions() const override;

    /**
     * @copydoc AccessibleNode::doAction()
     */
    bool doAction(std::string action) override;

    /**
     * @copydoc AccessibleNode::setValue()
     */
    bool setValue(std::string text) override;

    /**
     * @copydoc AccessibleNode::setValue()
     */
    bool setValue(double value) override;

    /**
     * @copydoc AccessibleNode::setIncludeHidden()
     */
    void setIncludeHidden(bool enabled) const override;

    /**
     * @copydoc AccessibleNode::getIncludeHidden()
     */
    bool getIncludeHidden() const override;

private:
    using AccessibleNode::setFeatureProperty;

    /**
     * @copydoc AccessibleNode::setFeatureProperty()
     */
    void setFeatureProperty(AtspiStateType type);

private:
    AtspiAccessible *mNode;
};

}

#endif
