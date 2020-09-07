#include "MockAccessibleApplication.h"
#include "MockAccessibleWindow.h"
#include "MockAccessibleNode.h"

#include <algorithm>

#include <loguru.hpp>

MockAccessibleApplication::MockAccessibleApplication(std::shared_ptr<AccessibleNode> node)
: AccessibleApplication(node), mWindowList{}
{
}

MockAccessibleApplication::~MockAccessibleApplication()
{
}

std::vector<std::shared_ptr<AccessibleWindow>> MockAccessibleApplication::getWindows(void)
{
    return mWindowList;
}

std::string MockAccessibleApplication::getPackageName(void)
{
    return "This.Is.Mock.Application";
}

void MockAccessibleApplication::addWindow(std::shared_ptr<AccessibleWindow> window)
{
    mWindowList.push_back(window);
}
void MockAccessibleApplication::clearWindows(void)
{
    mWindowList.clear();
}

std::shared_ptr<MockAccessibleWindow> MockAccessibleApplication::addWindow(std::string title, std::string widget, Rect<int> geometry, int properties)
{
    auto node = std::make_shared<MockAccessibleNode>(nullptr, title, title, "window", "res",widget,"style", "", geometry, 0, properties);
    auto win = std::make_shared<MockAccessibleWindow>(shared_from_this(), node);
    this->addWindow(win);
    return win;
}

