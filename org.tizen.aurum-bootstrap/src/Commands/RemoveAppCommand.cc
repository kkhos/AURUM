#include "bootstrap.h"
#include "RemoveAppCommand.h"
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
    LOGI("RemoveAppCommand::execute");

#ifdef GBSBUILD
    package_manager_request_h pkgRequest;
    std::string               name = mRequest->packagename();
    int                       id;
    LOGI("package name :%s", name.c_str());

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
     LOGI("RemoveAppCommand::executePost");
    std::this_thread::sleep_for(std::chrono::milliseconds{5000});
    return grpc::Status::OK;
}
