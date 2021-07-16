#include "Aurum.h"
#include "bootstrap.h"
#include "LaunchAppCommand.h"
#include "LaunchAppRunnable.h"

#ifdef GBSBUILD
#include <app_control.h>
#endif

LaunchAppRunnable::LaunchAppRunnable(std::string pkg)
    : mPkg{pkg}
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
        LOGE("Launch Failed(1/3) Err Code : %ull", ret);
        return;
    }

    ret = app_control_set_app_id(appControl, packageName.c_str());
    if (ret) {
        LOGE("Launch Failed(2/3) Err Code : %ull", ret);
        app_control_destroy(appControl);
        return;
    }

    ret = app_control_send_launch_request(appControl, NULL, NULL);
    if (ret) {
        LOGE("Launch Failed(3/3) Err Code : %ull", ret);
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