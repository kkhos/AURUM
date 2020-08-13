#pragma once
#include "AccessibleNode.h"
#include "AccessibleWatcher.h"

#include <mutex>
#include <shared_mutex>
#include <memory>
#include <list>
#include <vector>
#include <set>
#include <map>


class MockAccessibleWatcher : public AccessibleWatcher {
public:
    MockAccessibleWatcher();
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

public:
    void addApplication(std::shared_ptr<AccessibleApplication> application);

private:
    std::vector<std::shared_ptr<AccessibleApplication>> mApplicationList;

    /**
     * @brief TBD
     */
    std::mutex                            mLock;

};