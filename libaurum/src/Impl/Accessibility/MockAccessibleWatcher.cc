#include "MockAccessibleWatcher.h"
#include "MockAccessibleWindow.h"
#include "MockAccessibleApplication.h"
#include "MockAccessibleNode.h"

MockAccessibleWatcher::MockAccessibleWatcher()
{
}

MockAccessibleWatcher::~MockAccessibleWatcher()
{
}

int MockAccessibleWatcher::getApplicationCount(void) const
{
    return mApplicationList.size();
}

std::shared_ptr<AccessibleApplication> MockAccessibleWatcher::getApplicationAt(int index) const
{
    return mApplicationList.at(index);
}

std::vector<std::shared_ptr<AccessibleApplication>> MockAccessibleWatcher::getApplications(void) const
{
    return mApplicationList;
}

void MockAccessibleWatcher::addApplication(std::shared_ptr<AccessibleApplication> application)
{
    mApplicationList.push_back(application);
}

std::shared_ptr<MockAccessibleApplication> MockAccessibleWatcher::addApplication(std::string pkgName, Rect<int> geometry, int ifaces, int properties)
{
    std::shared_ptr<MockAccessibleNode> appNode = std::make_shared<MockAccessibleNode>(nullptr, pkgName, pkgName, "application", "", "Elm_Win", "default", "", geometry, ifaces, properties);
    std::shared_ptr<MockAccessibleApplication> app = std::make_shared<MockAccessibleApplication>(appNode);
    this->addApplication(app);
    return app;
}