#pragma once

#include "AccessibleWindow.h"
#include "MockAccessibleNode.h"

class MockAccessibleApplication;

class MockAccessibleWindow : public AccessibleWindow {
public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    MockAccessibleWindow(std::shared_ptr<MockAccessibleApplication> app, std::shared_ptr<MockAccessibleNode> node);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    ~MockAccessibleWindow();

public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::shared_ptr<MockAccessibleNode> addNode(std::string text, std::string pkg, std::string role, std::string res, std::string type, std::string style, std::string automationId, Rect<int> geometry, int ifaces, int properties);

private:
    /**
     * @brief TBD
     */
    std::shared_ptr<MockAccessibleNode> mMockNode;
};