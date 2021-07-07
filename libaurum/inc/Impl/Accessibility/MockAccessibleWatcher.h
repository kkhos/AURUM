#pragma once

#include "AccessibleNode.h"
#include "AccessibleWatcher.h"

#include "MockAccessibleApplication.h"

#include <mutex>
#include <shared_mutex>
#include <memory>
#include <vector>

class MockAccessibleWatcher : public AccessibleWatcher {
public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    MockAccessibleWatcher();

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual ~MockAccessibleWatcher();

public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual int getApplicationCount(void) const override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual std::shared_ptr<AccessibleApplication> getApplicationAt(int index) const override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual std::vector<std::shared_ptr<AccessibleApplication>> getApplications(void) const override;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    virtual bool executeAndWaitForEvents(const Runnable *cmd, const A11yEvent type, const int timeout) override;

public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    void addApplication(std::shared_ptr<AccessibleApplication> application);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::shared_ptr<MockAccessibleApplication> addApplication(std::string pkgName, Rect<int> geometry, int ifaces, int properties);

private:
    /**
     * @brief TBD
     */
    std::vector<std::shared_ptr<AccessibleApplication>> mApplicationList;
};
