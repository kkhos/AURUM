#ifndef ACCESSIBLE_H
#define ACCESSIBLE_H


#include "AccessibleApplication.h"
#include "AccessibleWindow.h"
#include "AccessibleNode.h"

#include "AccessibleUtils.h"

#include <memory>
#include <vector>


#include "config.h"


/**
 * @brief AccessibleWatcher class
 * @since_tizen 5.5
 */
class AccessibleWatcher {
public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual ~AccessibleWatcher();

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    static const AccessibleWatcher *getInstance(AccessibleWatcher *watcherImpl = nullptr);

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

public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual std::vector<std::shared_ptr<AccessibleApplication>> getActiveApplications(void) const;
};

#endif
