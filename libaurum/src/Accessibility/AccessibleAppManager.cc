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

#include "Aurum.h"
#include <algorithm>

#ifdef TIZEN
#include "AtspiAccessibleAppManager.h"
#endif
#include "MockAccessibleAppManager.h"

using namespace Aurum;
using namespace AurumInternal;

AccessibleAppManager::AccessibleAppManager()
{
}

AccessibleAppManager::~AccessibleAppManager()
{
}

AccessibleAppManager *AccessibleAppManager::getInstance(AccessibleAppManager *appManagerImpl)
{
    static AccessibleAppManager *mInstance = nullptr;
    if (appManagerImpl) {
        delete mInstance;
        mInstance = appManagerImpl;
        return mInstance;
    } else {
        if (mInstance) return mInstance;
        else {
#ifdef TIZEN
            mInstance = new AtspiAccessibleAppManager();
#else
            mInstance = new MockAccessibleAppManager();
#endif
        }
    }
    return mInstance;
}

std::vector<std::shared_ptr<AccessibleApplication>> AccessibleAppManager::getActiveApplications(void) const
{
    std::vector<std::shared_ptr<AccessibleApplication>> ret{};
    auto apps = this->getApplications();
    LOGI("available app process size %d", (int)apps.size());

    apps.erase(std::remove_if(apps.begin(), apps.end(), [](auto app){
        return !app->isActive();
    }), apps.end());

    LOGI("active apps size %d", (int)apps.size());

    return apps;
}
