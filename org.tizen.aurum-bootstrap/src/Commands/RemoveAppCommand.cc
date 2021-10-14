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
#include "RemoveAppCommand.h"
#include <chrono>
#include <thread>
#include <package_manager.h>

RemoveAppCommand::RemoveAppCommand(const ::aurum::ReqRemoveApp* request,
                                   ::aurum::RspRemoveApp*       response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status RemoveAppCommand::execute()
{
    LOGI("RemoveAppCommand::execute");

    package_manager_request_h pkgRequest;
    std::string               name = mRequest->packagename();
    int                       id;
    LOGI("package name :%s", name.c_str());

    if (package_manager_request_create(&pkgRequest) == PACKAGE_MANAGER_ERROR_NONE) {
        if (package_manager_request_uninstall(pkgRequest, name.c_str(), &id) == PACKAGE_MANAGER_ERROR_NONE) {
            mResponse->set_status(::aurum::RspStatus::OK);
        }
    }

    return grpc::Status::OK;
}

::grpc::Status RemoveAppCommand::executePost()
{
    LOGI("Wait for 5000ms");
    std::this_thread::sleep_for(std::chrono::milliseconds{5000});
    return grpc::Status::OK;
}
