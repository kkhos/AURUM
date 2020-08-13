#include "AccessibleWindow.h"
#include "AccessibleWatcher.h"

AccessibleWindow::AccessibleWindow(std::shared_ptr<AccessibleApplication> app, std::shared_ptr<AccessibleNode> node)
: mApp{app}, mNode{node}
{
}

AccessibleWindow::~AccessibleWindow()
{
}

std::string AccessibleWindow::getTitle(void)
{
    return mNode->getText();
}

bool AccessibleWindow::isShowing()
{
    return mNode->isShowing();
}

bool AccessibleWindow::isActive()
{
    return mNode->isActive();
}

std::shared_ptr<AccessibleApplication> AccessibleWindow::getApplication(void)
{
    return mApp;
}

std::shared_ptr<AccessibleNode> AccessibleWindow::getNode(void)
{
    return mNode;
}