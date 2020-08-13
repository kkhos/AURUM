#pragma once

#include "AccessibleWindow.h"
#include "AccessibleNode.h"

#include <memory>

class AccessibleApplication {
public:
    AccessibleApplication(std::shared_ptr<AccessibleNode> node);
    virtual ~AccessibleApplication();

public:
    std::shared_ptr<AccessibleNode> getAccessibleNode();
    bool isActive(void);

public:
    virtual std::vector<std::shared_ptr<AccessibleWindow>> getWindows(void) = 0;
    virtual std::vector<std::shared_ptr<AccessibleWindow>> getActiveWindows(void);
    virtual std::string getPackageName(void) = 0;

private:
    std::shared_ptr<AccessibleNode> mNode;
};

