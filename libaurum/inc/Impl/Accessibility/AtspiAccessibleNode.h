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

#ifndef _ATSPI_ACCESSIBLE_NODE_H_
#define _ATSPI_ACCESSIBLE_NODE_H_

#include "AccessibleNode.h"
#include <atspi/atspi.h>

using namespace Aurum;

namespace AurumInternal {

class AtspiAccessibleNode : public AccessibleNode {
public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    AtspiAccessibleNode(AtspiAccessible *node);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
   ~AtspiAccessibleNode() override;

public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    int getChildCount() const override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::shared_ptr<AccessibleNode> getChildAt(int index) const override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::vector<std::shared_ptr<AccessibleNode>> getChildren() const override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::shared_ptr<AccessibleNode> getParent() const override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool isValid() const override;

public:
    /**
     * @brief TBD
     * @since_tizen 5.5
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
     * @since_tizen 5.5
     */
    void refresh() override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::vector<std::string> getActions() const override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool doAction(std::string action) override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    void setValue(std::string text) override;

private:
    using AccessibleNode::setFeatureProperty;
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    void setFeatureProperty(AtspiStateType type);

private:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    AtspiAccessible *mNode;
};

}

#endif
