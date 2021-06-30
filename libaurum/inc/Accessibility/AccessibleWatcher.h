#pragma once

#include "AccessibleApplication.h"
#include "AccessibleWindow.h"
#include "AccessibleNode.h"
#include "AccessibleUtils.h"
#include "IEventSource.h"
#include "Runnable.h"
#include "A11yEvent.h"

#include <memory>
#include <vector>
#include <set>
#include <mutex>

#include "config.h"

/**
 * @brief AccessibleWatcher class
 * @since_tizen 5.5
 */
class AccessibleWatcher : public IEventSource {
public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    AccessibleWatcher();

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual ~AccessibleWatcher();

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    static AccessibleWatcher *getInstance(AccessibleWatcher *watcherImpl = nullptr);

public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual int getApplicationCount(void) const = 0;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual std::shared_ptr<AccessibleApplication> getApplicationAt(int index) const = 0;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual std::vector<std::shared_ptr<AccessibleApplication>> getApplications(void) const = 0;

    /**s
     * @brief TBD
     * @since_tizen 6.5
     */
    virtual bool executeAndWaitForEvents(const Runnable *cmd, const A11yEvent type, const int timeout) = 0;

public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual std::vector<std::shared_ptr<AccessibleApplication>> getActiveApplications(void) const;
public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    void attach(std::shared_ptr<IEventConsumer> source) override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    void detach(std::shared_ptr<IEventConsumer> source) override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    void notifyAll(int type, int type2, void *src) override;

private:
    /**
     * @brief TBD
     */
    std::set<std::shared_ptr<IEventConsumer>> mSources;

    /**
     * @brief TBD
     */
    std::mutex mLock;
};
