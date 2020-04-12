#include "CloseAppCommand.h"
#include <loguru.hpp>
#include <chrono>
#include <thread>
#ifdef GBSBUILD
#include <app_manager_extension.h>
#endif

CloseAppCommand::CloseAppCommand(const ::aurum::ReqCloseApp* request,
                                 ::aurum::RspCloseApp*       response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status CloseAppCommand::execute()
{
    LOG_SCOPE_F(INFO, "CloseApp --------------- ");
#ifdef GBSBUILD
    std::string   packageName = mRequest->packagename();
    app_context_h app_context = NULL;

    LOG_F(INFO, "close req : %s", packageName.c_str());

    int ret = app_manager_get_app_context(packageName.c_str(), &app_context);
    if (ret) {
        LOG_SCOPE_F(INFO, "Terminate Failed(1/2) Err Code : %d", ret);
        mResponse->set_status(::aurum::RspStatus::ERROR);
        return grpc::Status::OK;
    }
    ret = app_manager_terminate_app(app_context);
    if (ret) {
        LOG_SCOPE_F(INFO, "Terminate Failed(2/2) Err Code : %d", ret);
        mResponse->set_status(::aurum::RspStatus::ERROR);
        return grpc::Status::OK;
    }
#endif
    return grpc::Status::OK;
}

::grpc::Status CloseAppCommand::executePost()
{
    LOG_SCOPE_F(INFO, "CloseAppCommand::executePost");
    std::this_thread::sleep_for(std::chrono::milliseconds{1500});
    return grpc::Status::OK;
}