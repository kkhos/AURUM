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

#include <string>

#include "ClearCommand.h"

ClearCommand::ClearCommand(const ::aurum::ReqClear *request,
                           ::aurum::RspClear *response)
    : mRequest{request}, mResponse{response}
{
}

bool ClearCommand::hasHintText(std::shared_ptr<UiObject> obj)
{
    if (!obj) return false;

    auto old_text = obj->getText();
    obj->setText("");
    if (!old_text.compare(obj->getText())) {
        return true;
    } else {
        obj->setText(old_text);
        return false;
    }
}

::grpc::Status ClearCommand::execute()
{
    LOGI("Clear --------------- ");

    ObjectMapper *mObjMap = ObjectMapper::getInstance();
    std::shared_ptr<UiObject> obj = mObjMap->getElement(mRequest->elementid());

    if (obj) {
        obj->setText("");
        obj->updateName();
        auto text = obj->getText();
        if (text.length() != 0) {
            if (hasHintText(obj)) {
                mResponse->set_status(::aurum::RspStatus::OK);
            } else {
                mResponse->set_status(::aurum::RspStatus::ERROR);
            }
        } else {
            mResponse->set_status(::aurum::RspStatus::OK);
        }
    }

    return grpc::Status::OK;
}
