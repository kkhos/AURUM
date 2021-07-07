#pragma once

#include "AccessibleApplication.h"

#include <memory>
#include <atspi/atspi.h>

class AtspiAccessibleApplication : public AccessibleApplication, public std::enable_shared_from_this<AtspiAccessibleApplication> {
public:
    /**
     * @brief TBD
     * @since_tizen 6.0
     */
    AtspiAccessibleApplication(std::shared_ptr<AccessibleNode> node);

    /**
     * @brief TBD
     * @since_tizen 6.0
     */
    virtual ~AtspiAccessibleApplication();

public:
    /**
     * @brief TBD
     * @since_tizen 6.0
     */
    std::vector<std::shared_ptr<AccessibleWindow>> getWindows(void) override;

    /**
     * @brief TBD
     * @since_tizen 6.0
     */
    std::string getPackageName(void) override;
};