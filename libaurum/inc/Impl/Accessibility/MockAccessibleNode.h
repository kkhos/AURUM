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

#ifndef _MOCK_ACCESSIBLE_NODE_H_
#define _MOCK_ACCESSIBLE_NODE_H_

#include "AccessibleNode.h"

#include <mutex>
#include <set>

using namespace Aurum;

namespace AurumInternal {

namespace Mock {

class MockAccessibleNode : public AccessibleNode {
public:
    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    MockAccessibleNode(std::shared_ptr<AccessibleNode> parent, std::string text,std::string pkg,std::string role, std::string id, std::string type,std::string style, std::string automationId, Rect<int> screenBoundingBox,int supportingIfaces,int featureProperty);

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
   ~MockAccessibleNode() override;

public:
    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    int getChildCount() const override;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    std::shared_ptr<AccessibleNode> getChildAt(int index) const override;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    std::vector<std::shared_ptr<AccessibleNode>> getChildren() const override;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    std::shared_ptr<AccessibleNode> getParent() const override;

    /**
     * @brief TBD
     * @since_tizen 8.0
     */
    std::vector<std::shared_ptr<AccessibleNode>> getMatches(const std::shared_ptr<UiSelector> selector, const bool ealryReturn) const override;

    /**
     * @copydoc UiObject::getMatchesInMatches()
     */
    std::vector<std::shared_ptr<AccessibleNode>> getMatchesInMatches(const std::shared_ptr<UiSelector> firstSelector, const std::shared_ptr<UiSelector> secondSelector, const bool ealryReturn) const override;

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

public:
    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    void* getRawHandler(void) const override;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    void updateRoleName() override;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    void updateUniqueId() override;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    void updateName() override;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    void updateApplication() override;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    void updateAttributes() override;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    void updateStates() override;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    void updateExtents() override;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    void updateXPath() override;

    /**
     * @brief TBD
     * @since_tizen 7.0
     */
    void updateToolkitName() override;

    /**
     * @brief TBD
     * @since_tizen 7.0
     */
    void updateValue() override;

    /**
     * @brief TBD
     * @since_tizen 7.0
     */
    void updatePid() override;

    /**
     * @copydoc UiObject::updateInterface()
     */
    void updateInterface() override;

    /**
     * @brief TBD
     * @since_tizen 7.0
     */
    bool setFocus() override;

    /**
     * @brief TBD
     * @since_tizen 9.0
     */
    bool moveTo() override;

    /**
     * @brief TBD
     * @since_tizen 7.0
     */
    void updateTextMinBoundingRect() override;

	/**
     * @brief TBD
     * @since_tizen 6.5
     */
    void refresh(bool updateAll = true) override;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    std::vector<std::string> getActions() const override;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    bool doAction(std::string action) override;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    bool setValue(std::string text) override;

    /**
     * @brief TBD
     * @since_tizen 7.0
     */
    bool setValue(double value) override;

    /**
     * @brief TBD
     * @since_tizen 7.0
     */
    void setIncludeHidden(bool enabled) const override;

    /**
     * @brief TBD
     * @since_tizen 7.0
     */
    bool getIncludeHidden() const override;

public:
    using AccessibleNode::setFeatureProperty;
    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    void setFeatureProperty(int type);

public:
    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    void addChild(std::shared_ptr<AccessibleNode> child);

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    std::shared_ptr<MockAccessibleNode> addChild(std::string text, std::string pkg, std::string role, std::string res, std::string type, std::string style,std::string automationId,  Rect<int> geometry, int ifaces, int properties);

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    void clearChildren(void);

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    void addAction(std::string action);

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    void clearActions(void);

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    void setProperties(std::string text,std::string pkg, std::string role, std::string res, std::string type, std::string style,std::string automationId,  Rect<int> screenBoundingBox, int supportingIfaces, int featureProperty);

private:
    /**
     * @brief TBD
     */
    std::shared_ptr<AccessibleNode> mParentNode;

    /**
     * @brief TBD
     */
    std::vector<std::shared_ptr<AccessibleNode>> mChildrenList;

    /**
     * @brief TBD
     */
    std::set<std::string> mActionSet;

    /**
     * @brief TBD
     */
    std::mutex                            mLock;
};

}

}

#endif
