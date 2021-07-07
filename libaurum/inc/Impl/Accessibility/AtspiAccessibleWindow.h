#pragma once

#include "AccessibleWindow.h"

class AtspiAccessibleWindow : public AccessibleWindow {
public:
    /**
     * @brief TBD
     * @since_tizen 6.0
     */
    AtspiAccessibleWindow(std::shared_ptr<AccessibleApplication> app, std::shared_ptr<AccessibleNode> node);

    /**
     * @brief TBD
     * @since_tizen 6.0
     */
    ~AtspiAccessibleWindow();

};