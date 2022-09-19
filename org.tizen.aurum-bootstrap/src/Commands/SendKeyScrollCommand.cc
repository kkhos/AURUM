/*
 * Copyright (c) 2022 Samsung Electronics Co., Ltd All Rights Reserved
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
#include "SendKeyScrollCommand.h"
#include "UiDevice.h"

SendKeyScrollCommand::SendKeyScrollCommand(const ::aurum::ReqKeyScroll *request,
                               ::aurum::RspKeyScroll *response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status SendKeyScrollCommand::execute()
{
    LOGI("SendKeyScroll --------------- ");

    std::shared_ptr<UiDevice> mDevice = UiDevice::getInstance();
    ::aurum::ReqKeyScroll_KeyType type = mRequest->type();
    int intervalMs = mRequest->intervalms();
    int durationMs = mRequest->durationms();

    if (type == ::aurum::ReqKey_KeyType::ReqKey_KeyType_XF86) {
        mDevice->scrollKeyCode(mRequest->xf86keycode(), intervalMs, durationMs);
    }
    return grpc::Status::OK;
}