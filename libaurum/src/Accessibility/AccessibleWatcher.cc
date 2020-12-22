#include "AccessibleWatcher.h"

#ifdef TIZEN
#include "AtspiAccessibleWatcher.h"
#endif

#include "MockAccessibleWatcher.h"

#include <string.h>
#include <iostream>
#include <utility>
#include <algorithm>
#include <loguru.hpp>

AccessibleWatcher::AccessibleWatcher()
:mSources{}, mLock{}
{
}

AccessibleWatcher::~AccessibleWatcher()
{
}

/*
void AccessibleWatcher::printDbgInformation() const
{
    LOG_SCOPE_F(INFO, "%d %d", mActivatedWindowList.size(),  mWindowSet.size());

    for (auto iter = mActivatedWindowList.begin(); iter != mActivatedWindowList.end(); ++iter) {
        LOG_F(INFO, "%p", *iter);
    }
        LOG_F(INFO, "-----------");

    for (auto iter = mWindowSet.begin(); iter != mWindowSet.end(); ++iter) {
        LOG_F(INFO, "%p", *iter);
    }
}
*/

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
    LOG_SCOPE_F(INFO, "getActiveApplications for this(%p)", this);

    std::vector<std::shared_ptr<AccessibleApplication>> ret{};
    auto apps = this->getApplications();
    LOG_F(INFO, "apps size %d", apps.size());

    apps.erase(std::remove_if(apps.begin(), apps.end(), [](auto app){
        return !app->isActive();
    }), apps.end());

    LOG_F(INFO, "active apps size %d", apps.size());

    return apps;
}

void AccessibleWatcher::attach(std::shared_ptr<IEventConsumer> source)
{
    std::unique_lock<std::mutex> lock(mLock);
    LOG_F(INFO, "source attached %p", source.get());
    if (source) {
        mSources.insert(source);
    }
}

void AccessibleWatcher::detach(std::shared_ptr<IEventConsumer> source)
{
    std::unique_lock<std::mutex> lock(mLock);
    LOG_F(INFO, "source detached %p", source.get());
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
