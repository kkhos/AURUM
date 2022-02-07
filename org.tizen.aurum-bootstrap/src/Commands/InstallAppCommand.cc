/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *               http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#include "bootstrap.h"
#include "InstallAppCommand.h"
#include <fstream>
#include <chrono>
#include <thread>
#include <package_manager.h>

InstallAppCommand::InstallAppCommand(
    ::grpc::ServerReader<::aurum::ReqInstallApp> *request,
    ::aurum::RspInstallApp *                      response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status InstallAppCommand::execute()
{
    LOGI("InstallApp --------------- ");

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

    if (package_manager_request_create(&pkgRequest) != PACKAGE_MANAGER_ERROR_NONE) {
        LOGE("Could not create install request. App: %s", chunk.package().c_str());
        return grpc::Status::CANCELLED;;
    }
    if (package_manager_request_install(pkgRequest, "/tmp/app.tpk", &id) != PACKAGE_MANAGER_ERROR_NONE) {
        LOGE("Could not install application. App: %s", chunk.package().c_str());
        goto END;
    }
    if (package_manager_request_destroy(pkgRequest) != PACKAGE_MANAGER_ERROR_NONE) {
        pkgRequest = NULL;
        LOGE("Could not destroy install request. App: %s", chunk.package().c_str());
        goto END;
    }

    return grpc::Status::OK;

END:
    if (pkgRequest != NULL)
        package_manager_request_destroy(pkgRequest);

    return grpc::Status::CANCELLED;
}

::grpc::Status InstallAppCommand::executePost()
{
    LOGI("Wait for 5000ms");
    std::this_thread::sleep_for(std::chrono::milliseconds{5000});
    return grpc::Status::OK;
}