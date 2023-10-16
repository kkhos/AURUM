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

#include <chrono>
#include <thread>

#include "LaunchAppCommand.h"
#include "LaunchAppRunnable.h"
#include "UiDevice.h"

#define WAIT_APP_LAUNCH 10000

LaunchAppCommand::LaunchAppCommand(const ::aurum::ReqLaunchApp *request,
                                   ::aurum::RspLaunchApp *response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status LaunchAppCommand::execute()
{
    LOGI("LaunchApp (%s) --------------- ", mRequest->packagename().c_str());

    bool ret = false;

    std::unique_ptr<LaunchAppRunnable> cmd = std::make_unique<LaunchAppRunnable>(mRequest->packagename(), mRequest->data());
    std::shared_ptr<UiDevice> obj = UiDevice::getInstance();
    ret = obj->executeAndWaitForEvents(cmd.get(), A11yEvent::EVENT_WINDOW_ACTIVATE, WAIT_APP_LAUNCH, mRequest->packagename(), 0);

    if (ret) mResponse->set_status(::aurum::RspStatus::OK);
    else mResponse->set_status(::aurum::RspStatus::ERROR);

    return grpc::Status::OK;
}
