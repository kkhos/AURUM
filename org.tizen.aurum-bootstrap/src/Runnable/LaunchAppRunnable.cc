#include "Aurum.h"
#include "bootstrap.h"
#include "LaunchAppCommand.h"
#include "LaunchAppRunnable.h"

#ifdef GBSBUILD
#include <app_control.h>
#endif

LaunchAppRunnable::LaunchAppRunnable(std::string pkg, std::string key, std::string value)
    : mPkg{pkg}, mKey{key}, mValue{value}
{
}

void LaunchAppRunnable::run() const
{
#ifdef GBSBUILD
    app_control_h appControl;
    std::string   packageName = mPkg;
    int           ret = -1;

    if (packageName.empty()) return;

    ret = app_control_create(&appControl);
    if (ret) {
        LOGE("Launch Failed(1/4) Err Code : %ull", ret);
        return;
    }

    if (!mKey.empty() && !mValue.empty()) {
        ret = app_control_add_extra_data(appControl, mKey.c_str(), mValue.c_str());
        if (ret) {
            LOGE("Launch Failed(2/4) Err Code : %ull", ret);
            app_control_destroy(appControl);
            return;
        }
    }

    ret = app_control_set_app_id(appControl, packageName.c_str());
    if (ret) {
        LOGE("Launch Failed(3/4) Err Code : %ull", ret);
        app_control_destroy(appControl);
        return;
    }

    ret = app_control_send_launch_request(appControl, NULL, NULL);
    if (ret) {
        LOGE("Launch Failed(4/4) Err Code : %ull", ret);
        app_control_destroy(appControl);
        return;
    }

    app_control_destroy(appControl);
#endif
}

std::string LaunchAppRunnable::getPkgName()
{
    return mPkg;
}