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

#ifndef _MOCK_ACCESSIBLE_WATCHER_H_
#define _MOCK_ACCESSIBLE_WATCHER_H_

#include "AccessibleNode.h"
#include "AccessibleWatcher.h"

#include "MockAccessibleApplication.h"

#include <mutex>
#include <shared_mutex>
#include <memory>
#include <vector>

using namespace Aurum;

namespace AurumInternal {

namespace Mock {

class MockAccessibleWatcher : public AccessibleWatcher {
public:
    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    MockAccessibleWatcher();

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    virtual ~MockAccessibleWatcher();

public:
    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    virtual int getApplicationCount(void) const override;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    virtual std::shared_ptr<AccessibleApplication> getApplicationAt(int index) const override;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    virtual std::vector<std::shared_ptr<AccessibleApplication>> getApplications(void) const override;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    virtual bool executeAndWaitForEvents(const Runnable *cmd, const A11yEvent type, const int timeout, const std::string packageName, std::shared_ptr<AccessibleNode> obj)  override;

    virtual bool registerCallback(const A11yEvent type, EventHandler cb, void *data) override;

    std::map<AtspiAccessible *, std::shared_ptr<AccessibleApplication>> getActiveAppMap(void);

    std::map<std::string, std::shared_ptr<AurumXML>> getXMLDocMap(void);

public:
    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    void addApplication(std::shared_ptr<AccessibleApplication> application);

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    std::shared_ptr<MockAccessibleApplication> addApplication(std::string pkgName, Rect<int> geometry, int ifaces, int properties);

private:
    /**
     * @brief TBD
     */
    std::vector<std::shared_ptr<AccessibleApplication>> mApplicationList;

    std::map<AtspiAccessible *, std::shared_ptr<AccessibleApplication>> mActiveAppMap;
    std::map<std::string, std::shared_ptr<AurumXML>> mXMLDocMap;
};

}

}

#endif
