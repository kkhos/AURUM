#pragma once
#include "AccessibleWindow.h"

class MockAccessibleWindow : public AccessibleWindow {
public:
    MockAccessibleWindow(std::shared_ptr<AccessibleApplication> app, std::shared_ptr<AccessibleNode> node);
    ~MockAccessibleWindow();
};