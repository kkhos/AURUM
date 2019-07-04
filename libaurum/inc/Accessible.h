#ifndef ACCESSIBLE_H
#define ACCESSIBLE_H

#include <atspi/atspi.h>
#include "AccessibleNode.h"

#include <list>
#include <map>
#include <memory>
#include <vector>

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
    virtual void onActivate(AtspiAccessible *      node,
                            WindowActivateInfoType type) const = 0;
    virtual void onDeactivate(AtspiAccessible *node) const = 0;
    virtual void onVisibilityChanged(AtspiAccessible *node,
                                     bool             visible) const = 0;
    virtual void onObjectDefunct(AtspiAccessible *node) const = 0;
};

class Accessible : public IAtspiEvents {
private:
    Accessible();

public:
    static const Accessible *getInstance();
    virtual ~Accessible();

public:
    AccessibleNode *getRootNode() const;
    AccessibleNode *getTopNode() const;

    void onActivate(AtspiAccessible *      node,
                    WindowActivateInfoType type) const override;
    void onDeactivate(AtspiAccessible *node) const override;
    void onVisibilityChanged(AtspiAccessible *node,
                             bool             visible) const override;
    void onObjectDefunct(AtspiAccessible *node) const override;

private:
    void        clearWindowList() const;
    static void onAtspiWindowEvent(AtspiEvent *event, void *user_data);

private:
    static AtspiEventListener *                   listener;
    mutable std::list<AtspiAccessible *>          mWindowList;
    GDBusProxy *                                  mDbusProxy;
    std::map<AtspiAccessible *, AccessibleNode *> mAccessibleNode;
    mutable std::mutex                            mLock;
};

#endif
