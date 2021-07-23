#include "Aurum.h"
#include "bootstrap.h"
#include "LaunchAppCommand.h"
#include "LaunchAppRunnable.h"

#ifdef GBSBUILD
#include <app_control.h>
#endif

LaunchAppRunnable::LaunchAppRunnable(std::string pkg, const google::protobuf::RepeatedPtrField<aurum::LaunchData>& data)
    : mPkg{pkg}, mData(data)
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
        LOGE("Launch Failed(app_control_create) Err Code : %ull", ret);
        return;
    }

    std::for_each(mData.begin(), mData.end(), [&](auto data){
        ret = app_control_add_extra_data(appControl, data.key().c_str(),data.value().c_str());
        if (ret) {
            LOGE("Launch Failed(app_control_add_extra_data) Err Code : %ull", ret);
            app_control_destroy(appControl);
            return;
        }
    });

    ret = app_control_set_app_id(appControl, packageName.c_str());
    if (ret) {
        LOGE("Launch Failed(app_control_set_app_id) Err Code : %ull", ret);
        app_control_destroy(appControl);
        return;
    }

    ret = app_control_send_launch_request(appControl, NULL, NULL);
    if (ret) {
        LOGE("Launch Failed(app_control_send_launch_request) Err Code : %ull", ret);
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
