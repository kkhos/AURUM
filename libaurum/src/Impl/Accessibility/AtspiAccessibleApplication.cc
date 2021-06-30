#include "AtspiAccessibleApplication.h"
#include "AtspiAccessibleWindow.h"
#include "AtspiWrapper.h"

#include <algorithm>
#include <vector>

AtspiAccessibleApplication::AtspiAccessibleApplication(std::shared_ptr<AccessibleNode> node)
: AccessibleApplication(node)
{
}

AtspiAccessibleApplication::~AtspiAccessibleApplication()
{
}

std::vector<std::shared_ptr<AccessibleWindow>> AtspiAccessibleApplication::getWindows(void)
{
    std::vector<std::shared_ptr<AccessibleWindow>> ret{};
    auto children = getAccessibleNode()->getChildren();

    std::transform(children.begin(), children.end(), std::back_inserter(ret),
        [&](auto child) {
            return std::make_shared<AtspiAccessibleWindow>(this->shared_from_this(), child);
        }
    );

    return ret;
}

std::string AtspiAccessibleApplication::getPackageName(void)
{
    return getAccessibleNode()->getText();
}