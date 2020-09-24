#include "AccessibleApplication.h"
#include "AccessibleWatcher.h"

#include <algorithm>
#include <loguru.hpp>

AccessibleApplication::AccessibleApplication(std::shared_ptr<AccessibleNode> node)
: mNode{node}
{
}

AccessibleApplication::~AccessibleApplication()
{
}

std::shared_ptr<AccessibleNode> AccessibleApplication::getAccessibleNode()
{
    return mNode;
}

bool AccessibleApplication::isActive(void)
{
    auto children = this->getActiveWindows();
    return children.size() > 0;
}

std::vector<std::shared_ptr<AccessibleWindow>> AccessibleApplication::getActiveWindows(void)
{
    auto children = getWindows();

    children.erase(std::remove_if(children.begin(), children.end(), [](auto child){
                        return !(child->isActive() && child->isShowing()); // && child->isShowing() && child->isVisible());
                    }), children.end());

    return children;
}