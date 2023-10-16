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

#include "GetAngleCommand.h"
#include "UiDevice.h"

GetAngleCommand::GetAngleCommand(const ::aurum::ReqGetAngle* request,
                                   ::aurum::RspGetAngle*       response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status GetAngleCommand::execute()
{
    LOGI("GetAngle --------------- ");

    std::shared_ptr<UiDevice> mDevice = UiDevice::getInstance();

    mResponse->set_windowangle(mDevice->getWindowAngle());
    mResponse->set_targetangle(mDevice->getTargetAngle());
    mResponse->set_status(::aurum::RspStatus::OK);

    return grpc::Status::OK;
}
