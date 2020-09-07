#pragma once
#include "AccessibleWindow.h"
#include "MockAccessibleNode.h"

class MockAccessibleApplication;

class MockAccessibleWindow : public AccessibleWindow {
public:
    MockAccessibleWindow(std::shared_ptr<MockAccessibleApplication> app, std::shared_ptr<MockAccessibleNode> node);
    ~MockAccessibleWindow();
public:
    std::shared_ptr<MockAccessibleNode> addNode(std::string text, std::string pkg, std::string role, std::string res, std::string type, std::string style, std::string automationId, Rect<int> geometry, int ifaces, int properties);
private:
    std::shared_ptr<MockAccessibleNode> mMockNode;

};