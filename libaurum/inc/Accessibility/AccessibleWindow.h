#pragma once

#include "AccessibleNode.h"

#include <string>
#include <memory>

class AccessibleApplication;

class AccessibleWindow {
public:
    /**
     * @brief TBD
     * @since_tizen 6.0
     */
    AccessibleWindow(std::shared_ptr<AccessibleApplication> app, std::shared_ptr<AccessibleNode> node);

    /**
     * @brief TBD
     * @since_tizen 6.0
     */
    virtual ~AccessibleWindow();

public:
    /**
     * @brief TBD
     * @since_tizen 6.0
     */
    std::string getTitle(void);

    /**
     * @brief TBD
     * @since_tizen 6.0
     */
    bool isShowing();

    /**
     * @brief TBD
     * @since_tizen 6.0
     */
    bool isActive();

    /**
     * @brief TBD
     * @since_tizen 6.0
     */
    std::shared_ptr<AccessibleApplication> getApplication(void);

    /**
     * @brief TBD
     * @since_tizen 6.0
     */
    std::shared_ptr<AccessibleNode> getAccessibleNode(void);

private:
     /**
     * @brief TBD
     */
    std::shared_ptr<AccessibleApplication> mApp;

    /**
     * @brief TBD
     */
    std::shared_ptr<AccessibleNode> mNode;
};