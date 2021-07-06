#include <Aurum.h>
#include "LaunchAppCommand.h"
#include <chrono>
#include <thread>

#ifdef GBSBUILD
#include <app_control.h>
#endif

LaunchAppCommand::LaunchAppCommand(const ::aurum::ReqLaunchApp* request,
                                   ::aurum::RspLaunchApp*       response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status LaunchAppCommand::execute()
{
    LOGI("LaunchApp --------------- ");
#ifdef GBSBUILD
    app_control_h appControl;
    std::string   packageName = mRequest->packagename();
    int           ret = -1;

    if (packageName.empty()) return grpc::Status::OK;

    ret = app_control_create(&appControl);
    if (ret) {
        LOGI("Launch Failed(1/3) Err Code : %ull", ret);
        mResponse->set_status(::aurum::RspStatus::ERROR);
        return grpc::Status::OK;
    }

    ret = app_control_set_app_id(appControl, packageName.c_str());
    if (ret) {
        LOGI("Launch Failed(2/3) Err Code : %ull", ret);
        mResponse->set_status(::aurum::RspStatus::ERROR);
        app_control_destroy(appControl);
        return grpc::Status::OK;
    }

    ret = app_control_send_launch_request(appControl, NULL, NULL);
    if (ret) {
        LOGI("Launch Failed(3/3) Err Code : %ull", ret);
        mResponse->set_status(::aurum::RspStatus::ERROR);
        app_control_destroy(appControl);
        return grpc::Status::OK;
    }

    app_control_destroy(appControl);
#endif
    return grpc::Status::OK;
}
::grpc::Status LaunchAppCommand::executePost()
{
    LOGI("LaunchAppCommand::executePost");
    std::this_thread::sleep_for(std::chrono::milliseconds{2500});
    return grpc::Status::OK;
}
