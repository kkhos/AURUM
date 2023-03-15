/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *               http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#ifndef _ATSPI_ACCESSIBLE_WATCHER_H_
#define _ATSPI_ACCESSIBLE_WATCHER_H_

#include "AccessibleNode.h"
#include "AccessibleWatcher.h"
#include "AtspiAccessibleApplication.h"

#include <atspi/atspi.h>
#include <gio/gio.h>

#include <shared_mutex>
#include <memory>
#include <list>
#include <vector>
#include <set>
#include <map>
#include <mutex>
#include <condition_variable>

using namespace Aurum;

namespace AurumInternal {

/**
 * @internal
 *
 * @brief WindowActivateInfoType enum class.
 *
 * @since_tizen 6.5
 */
enum class WindowActivateInfoType {
    DEFAULT_LABEL_ENABLED = 0x00,
    DEFAULT_LABEL_ENABLED_WITHOUT_WINDOW = 0x01,
    DEFAULT_LABEL_DISABLED = 0x02,
    KEYBOARD = 0x04,
};

/**
 * @internal
 *
 * @brief Idle event state enum class.
 *
 * @since_tizen 7.5
 */
enum class IdleEventState {
    IDLE_LISTEN_START = 0x00,
    IDLE_LISTEN_READY = 0x01,
    IDLE_LISTEN_DONE = 0x02,
};

/**
 * @internal
 *
 * @brief IAtspiEvents Interface
 * @since_tizen 6.5
 */
class IAtspiEvents {
public:
    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    virtual ~IAtspiEvents() {}

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    virtual void onObjectDefunct(AtspiAccessible *node) = 0;
};

/**
 * @internal
 *
 * @class AtspiAccessibleWatcher
 *
 * @ingroup aurum
 *
 * @brief Class that communicates with the atspi sever and generates an event,
 *        and generates a node that matches with atspi node.
 */
class AtspiAccessibleWatcher : public AccessibleWatcher, public IAtspiEvents {
public:
    AtspiAccessibleWatcher();
    virtual ~AtspiAccessibleWatcher();

public:
    /**
     * @copydoc @AccessibleWatcher::getApplicationCount()
     */
    virtual int getApplicationCount(void) const override;

    /**
     * @copydoc @AccessibleWatcher::getApplicationAt()
     */
    virtual std::shared_ptr<AccessibleApplication> getApplicationAt(int index) const override;

    /**
     * @copydoc @AccessibleWatcher::getApplications()
     */
    virtual std::vector<std::shared_ptr<AccessibleApplication>> getApplications(void) const override;

    /**
     * @copydoc @AccessibleWatcher::executeAndWaitForEvents()
     */
    virtual bool executeAndWaitForEvents(const Runnable *cmd, const A11yEvent type, const int timeout, const std::string packageName, std::shared_ptr<AccessibleNode> obj, const int count)  override;

    /**
     * @copydoc @AccessibleWatcher::getActiveAppMap()
     */
    virtual std::map<AtspiAccessible *, std::shared_ptr<AccessibleApplication>> getActiveAppMap(void) override;

    /**
     * @copydoc @AccessibleWatcher::getXMLDocMap()
     */
    virtual std::map<std::string, std::shared_ptr<AurumXML>> getXMLDocMap(void) override;

    /**
     * @copydoc @AccessibleWatcher::getXMLDoc()
     */
    virtual std::shared_ptr<AurumXML> getXMLDoc(std::string pkgName) override;

    /**
     * @copydoc @AccessibleWatcher::registerCallback()
     */
    virtual bool registerCallback(const A11yEvent type, EventHandler cb, void *data) override;

public:
    /**
     * @brief Listen atspi events.
     *
     * @param[in] event AtspiEvent
     * @param[in] watcher @AtspiAccessibleWatcher
     *
     * @since_tizen 6.5
     */
    static void onAtspiEvents(AtspiEvent *event, void *watcher);

    /**
     * @brief Notifies when object defunct.
     *
     * @param[in] node @AtspiAccessible
     *
     * @since_tizen 6.5
     */
    void onObjectDefunct(AtspiAccessible *node) override;

private:
    /** Private methods for Mock Test **/
    bool removeFromActivatedList(AtspiAccessible *node);
    bool addToActivatedList(AtspiAccessible *node);
    bool removeFromWindowSet(AtspiAccessible *node);
    bool addToWindowSet(AtspiAccessible *node);
    static gpointer eventThreadLoop(gpointer data);
    static gpointer timerThread(gpointer data);
    void appendApp(AtspiAccessibleWatcher *instance, AtspiAccessible *app, char *pkg);
    void removeApp(AtspiAccessibleWatcher *instance, AtspiAccessible *app, char *pkg);
    void waitForXMLLoaded();

private:
    GDBusProxy *mDbusProxy;
    std::list<AtspiAccessible *> mActivatedWindowList;
    std::list<AtspiAccessible *> mActivatedApplicationList;
    std::set<AtspiAccessible *> mWindowSet;
    std::map<AtspiAccessible *, std::shared_ptr<AccessibleApplication>> mActiveAppMap;
    std::map<std::string, std::shared_ptr<AurumXML>> mXMLDocMap;
    static GThread *mEventThread;
    static std::vector<std::shared_ptr<A11yEventInfo>> mEventQueue;
    static std::mutex mMutex;
    static GMainLoop *mLoop;
    bool isTv;
    std::map<const A11yEvent, std::list<std::shared_ptr<A11yEventHandler>>> mHandlers;

    int mAppCount;

    // this variable should be protected by XMLMutex.
    int mAppXMLLoadedCount;

    std::mutex mXMLMutex;
    std::condition_variable mXMLConditionVar;

    static GThread *mTimerThread;
    static gint64 mStartTime;
    static IdleEventState isIdle;
    static int mRenderCount;
};

}

#endif
