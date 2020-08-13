#include "MockAccessibleApplication.h"
#include "MockAccessibleWindow.h"
#include "MockAccessibleNode.h"

#include <algorithm>

#include <loguru.hpp>

MockAccessibleApplication::MockAccessibleApplication(std::shared_ptr<AccessibleNode> node)
: AccessibleApplication(node), mWindowList{}
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
}

MockAccessibleApplication::~MockAccessibleApplication()
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
}

std::vector<std::shared_ptr<AccessibleWindow>> MockAccessibleApplication::getWindows(void)
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
    return mWindowList;
}
/*
std::vector<std::shared_ptr<AccessibleWindow>> MockAccessibleApplication::getActiveWindows(void)
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);

    std::vector<std::shared_ptr<AccessibleWindow>> ret{};

    auto children = getWindows();

    children.erase(std::remove_if(children.begin(), children.end(), [](auto child){
                        return !(child->isActive()); // && child->isShowing() && child->isVisible());
                    }), children.end());

    LOG_SCOPE_F(INFO, "getActiveWindows app(%s) for %p, size:%d", getPackageName().c_str(), getAccessibleNode()->getRawHandler(), children.size());

    // std::transform(children.begin(), children.end(), std::back_inserter(ret),
    //     [&](std::shared_ptr<AccessibleNode> child) {
    //         return std::make_shared<MockAccessibleWindow>(this->shared_from_this(), child);
    //     }
    // );

    return ret;
}
*/
std::string MockAccessibleApplication::getPackageName(void)
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
    return "This.Is.Mock.Application";
}

void MockAccessibleApplication::addWindow(std::shared_ptr<AccessibleWindow> window)
{
    mWindowList.push_back(window);
    // auto node = getAccessibleNode();
    // auto node2 = dynamic_cast<std::shared_ptr<MockAccessibleNode>>(node);
    // node2->addChild(window->getNode());
}
void MockAccessibleApplication::clearWindows(void)
{
    mWindowList.clear();
}