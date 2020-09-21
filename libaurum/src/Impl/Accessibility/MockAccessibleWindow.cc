#include "MockAccessibleWindow.h"
#include "MockAccessibleApplication.h"
#include "MockAccessibleNode.h"

MockAccessibleWindow::MockAccessibleWindow(std::shared_ptr<MockAccessibleApplication> app, std::shared_ptr<MockAccessibleNode> node)
: AccessibleWindow(app, node), mMockNode{node}
{
}

MockAccessibleWindow::~MockAccessibleWindow()
{
}

std::shared_ptr<MockAccessibleNode> MockAccessibleWindow::addNode(std::string text, std::string pkg, std::string role, std::string id, std::string type, std::string style, std::string automationId, Rect<int> geometry, int ifaces, int properties)
{
    auto node = std::make_shared<MockAccessibleNode>(mMockNode, text, pkg, role, id, type, style, automationId, geometry, ifaces, properties);
    mMockNode->addChild(node);
    return node;
}