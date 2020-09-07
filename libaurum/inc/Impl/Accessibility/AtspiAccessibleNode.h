#pragma once
#include "AccessibleNode.h"
#include <atspi/atspi.h>

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

public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    void* getRawHandler(void) const override;

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
    AtspiAccessible* mNode;
};