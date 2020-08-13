#pragma once

#include "AccessibleNode.h"

#include <string>
#include <memory>

class AccessibleApplication;

class AccessibleWindow {
public:
    AccessibleWindow(std::shared_ptr<AccessibleApplication> app, std::shared_ptr<AccessibleNode> node);
    virtual ~AccessibleWindow();

public:
    std::string getTitle(void);
    bool isShowing();
    bool isActive();

    std::shared_ptr<AccessibleApplication> getApplication(void);
    std::shared_ptr<AccessibleNode> getNode(void);

private:
    std::shared_ptr<AccessibleNode> mNode;
    std::shared_ptr<AccessibleApplication> mApp;
};