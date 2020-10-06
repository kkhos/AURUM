#include "RemoveAppCommand.h"
#include <loguru.hpp>
#include <chrono>
#include <thread>
#ifdef GBSBUILD
#include <package_manager.h>
#endif

RemoveAppCommand::RemoveAppCommand(const ::aurum::ReqRemoveApp* request,
                                   ::aurum::RspRemoveApp*       response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status RemoveAppCommand::execute()
{
    LOG_SCOPE_F(INFO, "RemoveAppCommand::execute");

#ifdef GBSBUILD
    package_manager_request_h pkgRequest;
    std::string               name = mRequest->packagename();
    int                       id;
    LOG_F(INFO, "package name :%s", name.c_str());

    if (package_manager_request_create(&pkgRequest) == PACKAGE_MANAGER_ERROR_NONE) {
        if (package_manager_request_uninstall(pkgRequest, name.c_str(), &id) == PACKAGE_MANAGER_ERROR_NONE) {
            mResponse->set_status(::aurum::RspStatus::OK);
        }
    }
#endif
    return grpc::Status::OK;
}

::grpc::Status RemoveAppCommand::executePost()
{
    LOG_SCOPE_F(INFO, "RemoveAppCommand::executePost");
    std::this_thread::sleep_for(std::chrono::milliseconds{5000});
    return grpc::Status::OK;
}
