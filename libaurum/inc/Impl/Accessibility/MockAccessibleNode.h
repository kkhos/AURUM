#pragma once
#include "AccessibleNode.h"

#include <mutex>
#include <set>

class MockAccessibleNode : public AccessibleNode {
public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    MockAccessibleNode(std::shared_ptr<AccessibleNode> parent, std::string text,std::string pkg,std::string role, std::string id, std::string type,std::string style, std::string automationId, Rect<int> boundingBox,int supportingIfaces,int featureProperty);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
   ~MockAccessibleNode() override;

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

public:
    using AccessibleNode::setFeatureProperty;
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    void setFeatureProperty(int type);

public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    void addChild(std::shared_ptr<AccessibleNode> child);
    std::shared_ptr<MockAccessibleNode> addChild(std::string text, std::string pkg, std::string role, std::string res, std::string type, std::string style,std::string automationId,  Rect<int> geometry, int ifaces, int properties);
    void clearChildren(void);
    void addAction(std::string action);
    void clearActions(void);
    void setProperties(std::string text,std::string pkg, std::string role, std::string res, std::string type, std::string style,std::string automationId,  Rect<int> boundingBox, int supportingIfaces, int featureProperty);

private:

    std::shared_ptr<AccessibleNode> mParentNode;
    std::vector<std::shared_ptr<AccessibleNode>> mChildrenList;
    std::set<std::string> mActionSet;
};


