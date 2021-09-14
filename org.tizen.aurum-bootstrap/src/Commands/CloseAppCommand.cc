#include "bootstrap.h"
#include "CloseAppCommand.h"
#include <chrono>
#include <thread>
#include <app_manager_extension.h>

CloseAppCommand::CloseAppCommand(const ::aurum::ReqCloseApp *request,
                                 ::aurum::RspCloseApp *response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status CloseAppCommand::execute()
{
    LOGI("CloseApp --------------- ");

    std::string   packageName = mRequest->packagename();
    app_context_h app_context = NULL;

    LOGI("close req : %s", packageName.c_str());

    int ret = app_manager_get_app_context(packageName.c_str(), &app_context);
    if (ret) {
        LOGI("Terminate Failed(1/2) Err Code : %d", ret);
        mResponse->set_status(::aurum::RspStatus::ERROR);
        return grpc::Status::OK;
    }
    ret = app_manager_terminate_app(app_context);
    if (ret) {
        LOGI("Terminate Failed(2/2) Err Code : %d", ret);
        mResponse->set_status(::aurum::RspStatus::ERROR);
        return grpc::Status::OK;
    }

    return grpc::Status::OK;
}

::grpc::Status CloseAppCommand::executePost()
{
    LOGI("Wait for 1500ms");
    std::this_thread::sleep_for(std::chrono::milliseconds{1500});
    return grpc::Status::OK;
}
