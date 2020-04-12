#include "RemoveAppCommand.h"
#include <loguru.hpp>
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
    LOG_SCOPE_F(INFO, "RemoveApp --------------- ");
#ifdef GBSBUILD
    package_manager_request_h pkgRequest;
    std::string               name = mRequest->packagename();
    int                       id;
    LOG_F(INFO, "package name :%s", name.c_str());

    package_manager_request_create(&pkgRequest);
    package_manager_request_uninstall(pkgRequest, name.c_str(), &id);
#endif

    return grpc::Status::OK;
}