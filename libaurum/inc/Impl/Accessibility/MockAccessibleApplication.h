#pragma once
#include "AccessibleApplication.h"
#include "MockAccessibleWindow.h"
#include "MockAccessibleNode.h"

#include <memory>

class MockAccessibleApplication : public AccessibleApplication, public std::enable_shared_from_this<MockAccessibleApplication> {
public:
    MockAccessibleApplication(std::shared_ptr<AccessibleNode> node);
    MockAccessibleApplication(std::shared_ptr<AccessibleNode> parent, std::string text,std::string pkg,std::string role, std::string res,std::string type,std::string style,Rect<int> boundingBox,int supportingIfaces,int featureProperty);
    virtual ~MockAccessibleApplication();

public:
    std::vector<std::shared_ptr<AccessibleWindow>> getWindows(void) override;
    std::string getPackageName(void) override;

public:
    void addWindow(std::shared_ptr<AccessibleWindow> window);
    std::shared_ptr<MockAccessibleWindow> addWindow(std::string title, std::string widget, Rect<int> geometry, int properties);
    void clearWindows(void);

private:
    std::vector<std::shared_ptr<AccessibleWindow>> mWindowList;

};