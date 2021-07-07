#pragma once

#include "AccessibleApplication.h"
#include "MockAccessibleWindow.h"
#include "MockAccessibleNode.h"

#include <memory>

class MockAccessibleApplication : public AccessibleApplication, public std::enable_shared_from_this<MockAccessibleApplication> {
public:
    /**
     * @brief TBD
     * @since tizen_6.0
     */
    MockAccessibleApplication(std::shared_ptr<AccessibleNode> node);

    /**
     * @brief TBD
     * @since tizen_6.0
     */
    MockAccessibleApplication(std::shared_ptr<AccessibleNode> parent, std::string text,std::string pkg,std::string role, std::string res,std::string type,std::string style,Rect<int> boundingBox,int supportingIfaces,int featureProperty);

    /**
     * @brief TBD
     * @since tizen_6.0
     */
    virtual ~MockAccessibleApplication();

public:
    /**
     * @brief TBD
     * @since tizen_6.0
     */
    std::vector<std::shared_ptr<AccessibleWindow>> getWindows(void) override;

    /**
     * @brief TBD
     * @since tizen_6.0
     */
    std::string getPackageName(void) override;

public:
    /**
     * @brief TBD
     * @since tizen_6.0
     */
    void addWindow(std::shared_ptr<AccessibleWindow> window);

    /**
     * @brief TBD
     * @since tizen_6.0
     */
    std::shared_ptr<MockAccessibleWindow> addWindow(std::string title, std::string widget, Rect<int> geometry, int properties);

    /**
     * @brief TBD
     * @since tizen_6.0
     */
    void clearWindows(void);

private:
    /**
     * @brief TBD
     */
    std::vector<std::shared_ptr<AccessibleWindow>> mWindowList;
};