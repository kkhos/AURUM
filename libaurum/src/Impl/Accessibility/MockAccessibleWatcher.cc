#include "MockAccessibleWatcher.h"
#include "MockAccessibleWindow.h"
#include "MockAccessibleApplication.h"
#include "MockAccessibleNode.h"

#include <stdio.h>

MockAccessibleWatcher::MockAccessibleWatcher()
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);

    /*
        Root
            Application
                Window
                    Node
                        Node
                            Node
                            Node
                            Node
                            Node
                        Node
                            Node
                            Node
                        Node
                    Node
                    Node
                Window
                    Node
                        Node
            Application
                Window
            Application
                Window
            Application
                Window
            Application
                Window
    */
}

MockAccessibleWatcher::~MockAccessibleWatcher()
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
}

int MockAccessibleWatcher::getApplicationCount(void) const
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
    return -1;
}

std::shared_ptr<AccessibleApplication> MockAccessibleWatcher::getApplicationAt(int index) const
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
    return mApplicationList.at(index);
}

std::vector<std::shared_ptr<AccessibleApplication>> MockAccessibleWatcher::getApplications(void) const
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
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