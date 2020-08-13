#pragma once
#include "AccessibleApplication.h"

#include <memory>

class MockAccessibleApplication : public AccessibleApplication, public std::enable_shared_from_this<MockAccessibleApplication> {
public:
    MockAccessibleApplication(std::shared_ptr<AccessibleNode> node);
    virtual ~MockAccessibleApplication();

public:
    std::vector<std::shared_ptr<AccessibleWindow>> getWindows(void) override;
    //std::vector<std::shared_ptr<AccessibleWindow>> getActiveWindows(void) override;
    std::string getPackageName(void) override;

public:
    void addWindow(std::shared_ptr<AccessibleWindow> window);
    void clearWindows(void);

private:
    std::vector<std::shared_ptr<AccessibleWindow>> mWindowList;

};