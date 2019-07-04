#include "GetAppInfoCommand.h"
#include <loguru.hpp>
#ifdef GBSBUILD
#include <app_manager_extension.h>
#include <package_manager.h>
#endif

GetAppInfoCommand::GetAppInfoCommand(const ::aurum::ReqGetAppInfo* request,
                                     ::aurum::RspGetAppInfo*       response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status GetAppInfoCommand::execute()
{
    LOG_SCOPE_F(INFO, "GetAppInfo --------------- ");
#ifdef GBSBUILD
    std::string packageName = mRequest->packagename();

    app_context_h  app_context;
    package_info_h package_info;
    app_state_e    appState;

    char* label = nullptr;
    bool  terminated = false;
    int   ret = -1;

    mResponse->set_status(::aurum::RspStatus::OK);

    package_manager_get_package_info(packageName.c_str(), &package_info);
    package_info_get_label(package_info, &label);
    if (label) {
        free(label);
        mResponse->set_isinstalled(true);
    } else {
        mResponse->set_isinstalled(false);
        return grpc::Status::OK;
    }

    ret = app_manager_get_app_context(packageName.c_str(), &app_context);
    if (ret) {
        mResponse->set_isrunning(false);
        return grpc::Status::OK;
    }

    ret = app_context_get_app_state(app_context, &appState);
    if (ret) {
        mResponse->set_status(::aurum::RspStatus::ERROR);
        return grpc::Status::OK;
    }
    mResponse->set_isrunning(!(appState & APP_STATE_TERMINATED));
    mResponse->set_isfocused(appState & APP_STATE_FOREGROUND);
#endif
    return grpc::Status::OK;
}