
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

const AccessibleWatcher *AccessibleWatcher::getInstance(AccessibleWatcher *watcherImpl)
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
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
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
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