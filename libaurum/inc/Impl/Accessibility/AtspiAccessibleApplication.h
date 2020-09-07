#pragma once
#include "AccessibleApplication.h"

#include <memory>
#include <atspi/atspi.h>

class AtspiAccessibleApplication : public AccessibleApplication, public std::enable_shared_from_this<AtspiAccessibleApplication> {
public:
    AtspiAccessibleApplication(std::shared_ptr<AccessibleNode> node);
    virtual ~AtspiAccessibleApplication();

public:
    std::vector<std::shared_ptr<AccessibleWindow>> getWindows(void) override;
    std::string getPackageName(void) override;
};