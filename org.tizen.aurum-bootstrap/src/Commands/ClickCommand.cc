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

#include "ClickCommand.h"
#include "UiObject.h"
#include "UiDevice.h"

ClickCommand::ClickCommand(const ::aurum::ReqClick *request,
                           ::aurum::RspClick *response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status ClickCommand::execute()
{
   return grpc::Status::CANCELLED;
}

std::unique_ptr<ClickCommand> ClickCommand::createCommand(const ::aurum::ReqClick *request, ::aurum::RspClick *response)
{
    ::aurum::ReqClick_RequestType type = request->type();

    if (type == ::aurum::ReqClick_RequestType::ReqClick_RequestType_ELEMENTID)
        return std::make_unique<ClickElementCommand>(request, response);
    else if (type == ::aurum::ReqClick_RequestType::ReqClick_RequestType_COORD)
        return std::make_unique<ClickCoordCommand>(request, response);
    else if (type == ::aurum::ReqClick_RequestType::ReqClick_RequestType_ATSPI)
        return std::make_unique<ClickAtspiCommand>(request, response);
    else
        return std::make_unique<ClickCommand>(request, response);
}

::grpc::Status ClickElementCommand::execute()
{
    LOGI("ClickElement --------------- ");

    ObjectMapper *mObjMap = ObjectMapper::getInstance();
    std::shared_ptr<UiObject> obj = mObjMap->getElement(mRequest->elementid());

    if (obj) {
        obj->click();
        mResponse->set_status(::aurum::RspStatus::OK);
    } else
        mResponse->set_status(::aurum::RspStatus::ERROR);

    return grpc::Status::OK;
}

::grpc::Status ClickCoordCommand::execute()
{
    std::shared_ptr<UiDevice> obj = UiDevice::getInstance();
    const ::aurum::Point& point = mRequest->coordination();

    LOGI("ClickCoord (%d, %d) --------------- ", point.x(), point.y());

    obj->click(point.x(), point.y());
    mResponse->set_status(::aurum::RspStatus::OK);

    return grpc::Status::OK;
}

::grpc::Status ClickAtspiCommand::execute()
{
    LOGI("ClickAtspi --------------- ");

    ObjectMapper *mObjMap = ObjectMapper::getInstance();
    std::shared_ptr<UiObject> obj = mObjMap->getElement(mRequest->elementid());

    if (obj) {
        if (obj->DoAtspiActivate()) mResponse->set_status(::aurum::RspStatus::OK);
        else mResponse->set_status(::aurum::RspStatus::ERROR);
    } else {
        mResponse->set_status(::aurum::RspStatus::ERROR);
    }

    return grpc::Status::OK;
}
