/*
 * Copyright (c) 2023 Samsung Electronics Co., Ltd All Rights Reserved
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

#include <app_manager_extension.h>
#include <package_manager.h>

#include "GetAppInfoCommand.h"

GetAppInfoCommand::GetAppInfoCommand(const ::aurum::ReqGetAppInfo *request,
                                     ::aurum::RspGetAppInfo *response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status GetAppInfoCommand::execute()
{
    LOGI("GetAppInfo (%s) --------------- ", mRequest->packagename().c_str());

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
        app_context_destroy(app_context);
    }

    return grpc::Status::OK;
}
