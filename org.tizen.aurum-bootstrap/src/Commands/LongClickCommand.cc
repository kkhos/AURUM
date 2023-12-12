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

#include "LongClickCommand.h"
#include "UiObject.h"
#include "UiDevice.h"

LongClickCommand::LongClickCommand(const ::aurum::ReqClick *request,
                                   ::aurum::RspClick *response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status LongClickCommand::execute()
{
    LOGI("LongClick --------------- ");

    ObjectMapper *mObjMap = ObjectMapper::getInstance();
    ::aurum::ReqClick_RequestType type = mRequest->type();

    if (type == ::aurum::ReqClick_RequestType_ELEMENTID) {
        std::shared_ptr<UiObject> obj = mObjMap->getElement(mRequest->elementid());
        if (obj) {
            obj->longClick(LONGCLICK_INTERVAL);
            mResponse->set_status(::aurum::RspStatus::OK);
        } else
            mResponse->set_status(::aurum::RspStatus::ERROR);
    } else if (type == ::aurum::ReqClick_RequestType_COORD) {
        std::shared_ptr<UiDevice> obj = UiDevice::getInstance();
        const ::aurum::Point& point = mRequest->coordination();
        obj->click(point.x(), point.y(), LONGCLICK_INTERVAL);
        mResponse->set_status(::aurum::RspStatus::OK);
    } else if (type == ::aurum::ReqClick_RequestType_ATSPI) {
        mResponse->set_status(::aurum::RspStatus::ERROR);
    }

    return grpc::Status::OK;
}