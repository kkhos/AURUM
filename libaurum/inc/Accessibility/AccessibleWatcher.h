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

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    virtual bool executeAndWaitForEvents(const Runnable *cmd, const A11yEvent type, const int timeout) = 0;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    virtual std::map<AtspiAccessible *, std::shared_ptr<AccessibleApplication>> getActiveAppMap(void) = 0;

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

}

#endif