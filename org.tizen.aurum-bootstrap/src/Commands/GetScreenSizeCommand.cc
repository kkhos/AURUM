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

#include "GetScreenSizeCommand.h"
#include "UiDevice.h"

GetScreenSizeCommand::GetScreenSizeCommand(const ::aurum::ReqGetScreenSize *request,
                               ::aurum::RspGetScreenSize *response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status GetScreenSizeCommand::execute()
{
    LOGI("GetScreenSize --------------- ");

    ::aurum::Rect *rect = mResponse->mutable_size();
    std::shared_ptr<UiDevice> mDevice = UiDevice::getInstance();

    rect->set_x(0);
    rect->set_y(0);
    rect->set_width(mDevice->getScreenSize().width);
    rect->set_height(mDevice->getScreenSize().height);

    return grpc::Status::OK;
}
