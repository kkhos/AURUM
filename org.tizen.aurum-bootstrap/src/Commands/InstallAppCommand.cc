#include "InstallAppCommand.h"
#include <fstream>
#include <chrono>
#include <thread>
#include <loguru.hpp>
#ifdef GBSBUILD
#include <package_manager.h>
#endif

InstallAppCommand::InstallAppCommand(
    ::grpc::ServerReader<::aurum::ReqInstallApp> *request,
    ::aurum::RspInstallApp *                      response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status InstallAppCommand::execute()
{
    LOG_SCOPE_F(INFO, "InstallApp --------------- ");
#ifdef GBSBUILD
    ::aurum::ReqInstallApp chunk;

    std::ofstream outfile("/tmp/app.tpk", std::ofstream::binary);

    while (mRequest->Read(&chunk)) {
        std::size_t size = chunk.package().length();
        const char *bufptr = chunk.package().c_str();
        outfile.write(bufptr, size);
    }
    outfile.close();

    package_manager_request_h pkgRequest;
    int                       id;

    package_manager_request_create(&pkgRequest);
    package_manager_request_install(pkgRequest, "/tmp/app.tpk", &id);
#endif
    return grpc::Status::OK;
}

::grpc::Status InstallAppCommand::executePost()
{
    LOG_SCOPE_F(INFO, "InstallAppCommand::executePost");
    std::this_thread::sleep_for(std::chrono::milliseconds{5000});
    return grpc::Status::OK;
}