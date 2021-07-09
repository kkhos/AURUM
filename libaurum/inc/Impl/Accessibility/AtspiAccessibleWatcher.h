#pragma once

#include "AccessibleNode.h"
#include "AccessibleWatcher.h"

#include <atspi/atspi.h>
#include <gio/gio.h>

#include <shared_mutex>
#include <memory>
#include <list>
#include <vector>
#include <set>
#include <map>

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
    virtual void onWindowActivated(AtspiAccessible *node, WindowActivateInfoType type) = 0;

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
    virtual void onVisibilityChanged(AtspiAccessible *node, bool visible) = 0;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual void onObjectDefunct(AtspiAccessible *node) = 0;
};


class AtspiAccessibleWatcher : public AccessibleWatcher, public IAtspiEvents {
public:
    AtspiAccessibleWatcher();
    virtual ~AtspiAccessibleWatcher();

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
    static void onAtspiEvents(AtspiEvent *event, void *user_data);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    void onWindowActivated(AtspiAccessible *node, WindowActivateInfoType type) override;

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
    void onVisibilityChanged(AtspiAccessible *node, bool visible) override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    void onObjectDefunct(AtspiAccessible *node) override;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    static void onEventListener(AtspiEvent *event, void *user_data);
private:
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

    void print_debug();

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    void addEventListener(AtspiEventListener *listener, A11yEvent type);

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    void removeEventListener(AtspiEventListener *listener, A11yEvent type);

public:
    /**
     * @brief TBD
     */
    static guint timeoutId;

private:
    /**
     * @brief TBD
     */
    GDBusProxy *                                  mDbusProxy;

    /**
     * @brief TBD
     */
    std::list<AtspiAccessible *>          mActivatedWindowList;

    /**
     * @brief TBD
     */
    std::list<AtspiAccessible *>          mActivatedApplicationList;

    /**
     * @brief TBD
     */
    std::set<AtspiAccessible *>            mWindowSet;

    /**
     * @brief TBD
     */
    std::map<AtspiAccessible *, AtspiAccessible *> mWindowAppMap;

    /**
     * @brief TBD
     */
    static GThread *                                      mEventThread;

    /**
     * @brief TBD
     */
    static std::vector<std::shared_ptr<A11yEventInfo>>    mEventQueue;

    static std::mutex mMutex;
};
