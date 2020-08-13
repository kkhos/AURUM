#pragma once
#include "AccessibleWindow.h"

class AtspiAccessibleWindow : public AccessibleWindow {
public:
    AtspiAccessibleWindow(std::shared_ptr<AccessibleApplication> app, std::shared_ptr<AccessibleNode> node);
    ~AtspiAccessibleWindow();

};