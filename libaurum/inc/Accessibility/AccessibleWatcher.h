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

#ifndef _ACCESSIBLE_WATCHER_H_
#define _ACCESSIBLE_WATCHER_H_

#include "AccessibleApplication.h"
#include "AccessibleWindow.h"
#include "AccessibleNode.h"
#include "IEventSource.h"
#include "Runnable.h"
#include "A11yEvent.h"
#include <atspi/atspi.h>

#include <memory>
#include <vector>
#include <set>
#include <mutex>

#include "config.h"

namespace Aurum {

/**
 * @class AccessibleWatcher
 *
 * @ingroup aurum
 *
 * @brief Class that communicates with the atspi sever and generates an event,
 *        and generates a node that matches with atspi node.
 */
class AccessibleWatcher : public IEventSource {
public:
    /**
     * @brief AccessibleWatcher constructor.
     *
     * @since_tizen 5.5
     */
    AccessibleWatcher();

    /**
     * @brief AccessibleWatcher destructor.
     *
     * @since_tizen 5.5
     */
    virtual ~AccessibleWatcher();

    /**
     * @brief Gets @AccessibleWatcher instance.
     *
     * @since_tizen 5.5
     */
    static AccessibleWatcher *getInstance(AccessibleWatcher *watcherImpl = nullptr);

public:
    /**
     * @brief Gets current running application count in device.
     *
     * @return application count
     *
     * @since_tizen 5.5
     */
    virtual int getApplicationCount(void) const = 0;

    /**
     * @brief Gets application class at specific index.
     *
     * @param[in] index application index
     *
     * @return @AccessibleApplication ptr
     *
     * @since_tizen 5.5
     */
    virtual std::shared_ptr<AccessibleApplication> getApplicationAt(int index) const = 0;

    /**
     * @brief Gets application node vector.
     *
     * @return @AccessibleApplication vector
     *
     * @since_tizen 5.5
     */
    virtual std::vector<std::shared_ptr<AccessibleApplication>> getApplications(void) const = 0;

    /**
     * @brief Executes runnable command and wait given event.
     *
     * @param[in] cmd @Runnable
     * @param[in] type @A11yEvent
     * @param[in] timeout time(millisecond) to wait event occur
     *
     * @return true if event occur in time, else false
     *
     * @since_tizen 6.5
     */
    virtual bool executeAndWaitForEvents(const Runnable *cmd, const A11yEvent type, const int timeout) = 0;

    /**
     * @brief Gets active application map.
     *
     * @return map (pure Atspi Node ptr(@AtspiAccessible), @AccessibleApplication)
     * @since_tizen 6.5
     */
    virtual std::map<AtspiAccessible *, std::shared_ptr<AccessibleApplication>> getActiveAppMap(void) = 0;

public:
    /**
     * @brief Gets active application vector.
     *
     * @return @AccessibleApplication vector
     *
     * @since_tizen 5.5
     */
    virtual std::vector<std::shared_ptr<AccessibleApplication>> getActiveApplications(void) const;

public:
    /**
     * @copydoc @IEventSource::attach()
     */
    void attach(std::shared_ptr<IEventConsumer> source) override;

    /**
     * @copydoc @IEventSource::detach()
     */
    void detach(std::shared_ptr<IEventConsumer> source) override;

    /**
     * @copydoc @IEventSource::notifyAll()
     */
    void notifyAll(int type1, int type2, void *src) override;

private:
    std::set<std::shared_ptr<IEventConsumer>> mSources;
    std::mutex mLock;
};

}

#endif
