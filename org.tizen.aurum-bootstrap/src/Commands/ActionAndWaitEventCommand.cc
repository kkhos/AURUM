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
#include "ActionAndWaitEventCommand.h"
#include "ActionAndWaitEventRunnable.h"
#include "UiDevice.h"
#include <chrono>
#include <thread>

ActionAndWaitEventCommand::ActionAndWaitEventCommand(const ::aurum::ReqActionAndWaitEvent *request,
                                   ::aurum::RspActionAndWaitEvent *response)
    : mRequest{request}, mResponse{response}
{
}

A11yEvent convertEventType(const ::aurum::ReqActionAndWaitEvent_EventType type)
{
    if (type == ::aurum::ReqActionAndWaitEvent_EventType::ReqActionAndWaitEvent_EventType_EVENT_WINDOW_ACTIVATE)
        return A11yEvent::EVENT_WINDOW_ACTIVATE;
    else if (type == ::aurum::ReqActionAndWaitEvent_EventType::ReqActionAndWaitEvent_EventType_EVENT_WINDOW_DEACTIVATE)
        return A11yEvent::EVENT_WINDOW_DEACTIVATE;
    else
        return A11yEvent::EVENT_STATE_CHANGED_FOCUSED;
}

::grpc::Status ActionAndWaitEventCommand::execute()
{
    LOGI("ActionAndWaitEvent --------------- ");
    std::unique_ptr<ActionAndWaitEventRunnable> cmd = std::make_unique<ActionAndWaitEventRunnable>(
                                                      mRequest->type(), mRequest->elementid(), mRequest->xf86keycode());
    std::shared_ptr<UiDevice> obj = UiDevice::getInstance();
    bool ret = obj->executeAndWaitForEvents(cmd.get(), convertEventType(mRequest->eventtype()), mRequest->timeoutms());

    if (ret) mResponse->set_status(::aurum::RspStatus::OK);
    else mResponse->set_status(::aurum::RspStatus::ERROR);

    return grpc::Status::OK;
}
