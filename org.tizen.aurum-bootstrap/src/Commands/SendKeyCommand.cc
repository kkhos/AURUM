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

#include "SendKeyCommand.h"
#include "UiDevice.h"

SendKeyCommand::SendKeyCommand(const ::aurum::ReqKey *request,
                               ::aurum::RspKey *response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status SendKeyCommand::execute()
{
    LOGI("SendKey --------------- ");

    std::shared_ptr<UiDevice> mDevice = UiDevice::getInstance();
    ::aurum::ReqKey_KeyType type = mRequest->type();
    ::aurum::ReqKey_KeyActionType action_type = mRequest->actiontype();
    KeyRequestType actionType = static_cast<KeyRequestType>(action_type);

    if (type == ::aurum::ReqKey_KeyType::ReqKey_KeyType_BACK)
        mDevice->pressBack(actionType);
    else if (type == ::aurum::ReqKey_KeyType::ReqKey_KeyType_HOME)
        mDevice->pressHome(actionType);
    else if (type == ::aurum::ReqKey_KeyType::ReqKey_KeyType_MENU)
        mDevice->pressMenu(actionType);
    else if (type == ::aurum::ReqKey_KeyType::ReqKey_KeyType_VOLUP)
        mDevice->pressVolUp(actionType);
    else if (type == ::aurum::ReqKey_KeyType::ReqKey_KeyType_VOLDOWN)
        mDevice->pressVolDown(actionType);
    else if (type == ::aurum::ReqKey_KeyType::ReqKey_KeyType_POWER)
        mDevice->pressPower(actionType);
    else if (type == ::aurum::ReqKey_KeyType::ReqKey_KeyType_WHEELUP)
        mDevice->wheelUp(1, 167);
    else if (type == ::aurum::ReqKey_KeyType::ReqKey_KeyType_WHEELDOWN)
        mDevice->wheelDown(1, 167);
    else if (type == ::aurum::ReqKey_KeyType::ReqKey_KeyType_XF86)
    {
        if (actionType == KeyRequestType::REPEAT)
        {
            int durationMs = mRequest->durationms();
            int intervalMs = mRequest->intervalms();
            mDevice->repeatKeyCode(mRequest->xf86keycode(), intervalMs, durationMs);
        }
        else
        {
            mDevice->pressKeyCode(mRequest->xf86keycode(), actionType);
        }
    }
    return grpc::Status::OK;
}