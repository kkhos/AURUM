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

#include "MockAccessibleWatcher.h"
#include "MockAccessibleWindow.h"
#include "MockAccessibleApplication.h"
#include "MockAccessibleNode.h"

using namespace Aurum;
using namespace AurumInternal;

MockAccessibleWatcher::MockAccessibleWatcher()
{
}

MockAccessibleWatcher::~MockAccessibleWatcher()
{
}

int MockAccessibleWatcher::getApplicationCount(void) const
{
    return mApplicationList.size();
}

std::shared_ptr<AccessibleApplication> MockAccessibleWatcher::getApplicationAt(int index) const
{
    return mApplicationList.at(index);
}

std::vector<std::shared_ptr<AccessibleApplication>> MockAccessibleWatcher::getApplications(void) const
{
    return mApplicationList;
}

void MockAccessibleWatcher::addApplication(std::shared_ptr<AccessibleApplication> application)
{
    mApplicationList.push_back(application);
}
std::map<AtspiAccessible *, std::shared_ptr<AccessibleApplication>> MockAccessibleWatcher::getActiveAppMap(void)
{
    return mActiveAppMap;
}

std::shared_ptr<MockAccessibleApplication> MockAccessibleWatcher::addApplication(std::string pkgName, Rect<int> geometry, int ifaces, int properties)
{
    std::shared_ptr<MockAccessibleNode> appNode = std::make_shared<MockAccessibleNode>(nullptr, pkgName, pkgName, "application", "", "Elm_Win", "default", "", geometry, ifaces, properties);
    std::shared_ptr<MockAccessibleApplication> app = std::make_shared<MockAccessibleApplication>(appNode);
    this->addApplication(app);
    return app;
}

bool MockAccessibleWatcher::executeAndWaitForEvents(const Runnable *cmd, const A11yEvent type, const int timeout)
{
    return true;
}
