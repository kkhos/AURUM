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



enum class WindowActivateInfoType {
    DEFAULT_LABEL_ENALBED = 0x00,
    DEFAULT_LABEL_ENALBED_WITHOUT_WINDOW = 0x01,
    DEFAULT_LABEL_DISABLED = 0x02,
    KEYBOARD = 0x04,
};

class IAtspiEvents {
public:
    virtual ~IAtspiEvents() {}
    virtual void onWindowActivated(AtspiAccessible *      node,
                                   WindowActivateInfoType type) = 0;
    virtual void onWindowDeactivated(AtspiAccessible *node) = 0;

    virtual void onWindowCreated(AtspiAccessible *node) = 0;
    virtual void onWindowDestroyed(AtspiAccessible *node) = 0;

    virtual void onVisibilityChanged(AtspiAccessible *node,
                                     bool             visible) = 0;
    virtual void onObjectDefunct(AtspiAccessible *node) = 0;
};

class AccessibleWatcher : public IAtspiEvents {
private:
    AccessibleWatcher();

public:
    static const AccessibleWatcher *getInstance();
    virtual ~AccessibleWatcher();

public:
    AccessibleNode *getRootNode() const;
    AccessibleNode *getTopNode() const;

    void onWindowActivated(AtspiAccessible *      node,
                                   WindowActivateInfoType type) override;
    void onWindowDeactivated(AtspiAccessible *node) override;

    void onWindowCreated(AtspiAccessible *node) override;
    void onWindowDestroyed(AtspiAccessible *node) override;

    void onVisibilityChanged(AtspiAccessible *node,
                                     bool             visible) override;
    void onObjectDefunct(AtspiAccessible *node) override;

    void printDbgInformation() const;

private:
    void        clearWindowList() const;
    static void onAtspiWindowEvent(AtspiEvent *event, void *user_data);

    bool removeFromActivatedList(AtspiAccessible *node);
    bool addToActivatedList(AtspiAccessible *node);
    bool removeFromWindowSet(AtspiAccessible *node);
    bool addToWindowSet(AtspiAccessible *node);

private:
    static AtspiEventListener *                   listener;
    mutable std::list<AtspiAccessible *>          mActivatedWindowList;
    mutable std::list<AtspiAccessible *>          mActivatedApplicationList;
    mutable std::set<AtspiAccessible *>           mWindowSet;;

    GDBusProxy *                                  mDbusProxy;
    std::map<AtspiAccessible *, AccessibleNode *> mAccessibleNode;
    mutable std::mutex                            mLock;
};

#endif
