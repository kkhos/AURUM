#ifndef ACCESSIBLE_H
#define ACCESSIBLE_H

#include <atspi/atspi.h>
#include "AccessibleNode.h"
#include "AccessibleUtils.h"

#include <list>
#include <map>
#include <memory>
#include <vector>
#include <set>


#include <gio/gio.h>
#include <mutex>
#include <shared_mutex>
#include "config.h"



/**
 * @brief WindowActivateInfoType enum class
 * @since_tizen 5.5
 */
enum class WindowActivateInfoType {
    DEFAULT_LABEL_ENALBED = 0x00,
    DEFAULT_LABEL_ENALBED_WITHOUT_WINDOW = 0x01,
    DEFAULT_LABEL_DISABLED = 0x02,
    KEYBOARD = 0x04,
};

/**
 * @brief IAtspiEvents Interface
 * @since_tizen 5.5
 */
class IAtspiEvents {
public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual ~IAtspiEvents() {}

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual void onWindowActivated(AtspiAccessible *      node,
                                   WindowActivateInfoType type) = 0;
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual void onWindowDeactivated(AtspiAccessible *node) = 0;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual void onWindowCreated(AtspiAccessible *node) = 0;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual void onWindowDestroyed(AtspiAccessible *node) = 0;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual void onVisibilityChanged(AtspiAccessible *node,
                                     bool             visible) = 0;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual void onObjectDefunct(AtspiAccessible *node) = 0;
};

/**
 * @brief AccessibleWatcher class
 * @since_tizen 5.5
 */
class AccessibleWatcher : public IAtspiEvents {
private:

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    AccessibleWatcher();

public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    static const AccessibleWatcher *getInstance();
        /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual ~AccessibleWatcher();

public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::unique_ptr<AccessibleNode> getRootNode() const;
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::vector<std::unique_ptr<AccessibleNode>> getTopNode() const;
    /**
     * @brief TBD
     * @since_tizen 5.5
     */

    void onWindowActivated(AtspiAccessible *      node,
                                   WindowActivateInfoType type) override;
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    void onWindowDeactivated(AtspiAccessible *node) override;
    /**
     * @brief TBD
     * @since_tizen 5.5
     */

    void onWindowCreated(AtspiAccessible *node) override;
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    void onWindowDestroyed(AtspiAccessible *node) override;
    /**
     * @brief TBD
     * @since_tizen 5.5
     */

    void onVisibilityChanged(AtspiAccessible *node,
                                     bool             visible) override;
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    void onObjectDefunct(AtspiAccessible *node) override;
    /**
     * @brief TBD
     * @since_tizen 5.5
     */

    void printDbgInformation() const;

private:
    void        clearWindowList() const;
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    static void onAtspiWindowEvent(AtspiEvent *event, void *user_data);
    /**
     * @brief TBD
     * @since_tizen 5.5
     */

    bool removeFromActivatedList(AtspiAccessible *node);
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool addToActivatedList(AtspiAccessible *node);
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool removeFromWindowSet(AtspiAccessible *node);
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool addToWindowSet(AtspiAccessible *node);


private:
    /**
     * @brief TBD
     */
    static AtspiEventListener *                   listener;

    /**
     * @brief TBD
     */
    mutable std::list<AtspiAccessible *>          mActivatedWindowList;

    /**
     * @brief TBD
     */
    mutable std::list<AtspiAccessible *>          mActivatedApplicationList;

    /**
     * @brief TBD
     */
    mutable std::set<AtspiAccessible *>           mWindowSet;;

    /**
     * @brief TBD
     */
    GDBusProxy *                                  mDbusProxy;

    /**
     * @brief TBD
     */
    std::map<AtspiAccessible *, AccessibleNode *> mAccessibleNode;

    /**
     * @brief TBD
     */
    mutable std::mutex                            mLock;
};

#endif
