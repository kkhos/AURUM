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

#ifdef TIZEN
#include "AtspiAccessibleWatcher.h"
#endif
#include "MockAccessibleWatcher.h"

#include <string.h>
#include <iostream>
#include <utility>
#include <algorithm>

using namespace Aurum;

AccessibleWatcher::AccessibleWatcher()
:mSources{}, mLock{}
{
}

AccessibleWatcher::~AccessibleWatcher()
{
}

AccessibleWatcher *AccessibleWatcher::getInstance(AccessibleWatcher *watcherImpl)
{
    static AccessibleWatcher *mInstance = nullptr;
    if (watcherImpl) {
        delete mInstance;
        mInstance = watcherImpl;
        return mInstance;
    } else {
        if (mInstance) return mInstance;
        else {
#ifdef TIZEN
            mInstance = new AtspiAccessibleWatcher();
#else
            mInstance = new MockAccessibleWatcher();
#endif
        }
    }
    return mInstance;
}

std::vector<std::shared_ptr<AccessibleApplication>> AccessibleWatcher::getActiveApplications(void) const
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

void AccessibleWatcher::attach(std::shared_ptr<IEventConsumer> source)
{
    std::unique_lock<std::mutex> lock(mLock);
    if (source) {
        mSources.insert(source);
    }
}

void AccessibleWatcher::detach(std::shared_ptr<IEventConsumer> source)
{
    std::unique_lock<std::mutex> lock(mLock);
    if (source) {
        auto iter = mSources.find(source);
        if (iter != mSources.end()) mSources.erase(iter);
    }
}

void AccessibleWatcher::notifyAll(int type, int type2, void *src)
{
    std::unique_lock<std::mutex> lock(mLock);
    std::for_each(mSources.begin(), mSources.end(), [&](auto source){
        source->notify(type, type2, src);
    });
}
