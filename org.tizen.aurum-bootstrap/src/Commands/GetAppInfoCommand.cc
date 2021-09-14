#include "bootstrap.h"
#include "GetAppInfoCommand.h"
#include <app_manager_extension.h>
#include <package_manager.h>

GetAppInfoCommand::GetAppInfoCommand(const ::aurum::ReqGetAppInfo *request,
                                     ::aurum::RspGetAppInfo *response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status GetAppInfoCommand::execute()
{
    LOGI("GetAppInfo --------------- ");

    std::string packageName = mRequest->packagename();

    app_context_h  app_context;
    package_info_h package_info;
    app_state_e    appState;

    char *label = nullptr;

    mResponse->set_status(::aurum::RspStatus::OK);
    mResponse->set_isinstalled(false);
    mResponse->set_isrunning(false);
    mResponse->set_isfocused(false);

    if (!package_manager_get_package_info(packageName.c_str(), &package_info)) {
        if (!package_info_get_label(package_info, &label)) {
            if (label) {
                free(label);
                mResponse->set_isinstalled(true);
            }
        }
        package_info_destroy(package_info);
    }

    if (!app_manager_get_app_context(packageName.c_str(), &app_context)) {
        if (!app_context_get_app_state(app_context, &appState)) {
            mResponse->set_isfocused(appState & APP_STATE_FOREGROUND);
            mResponse->set_isrunning(!(appState & APP_STATE_TERMINATED));
        }
    }

    return grpc::Status::OK;
}
