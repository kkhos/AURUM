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

#include "GetValueCommand.h"
#include "UiObject.h"

GetValueCommand::GetValueCommand(const ::aurum::ReqGetValue *request,
                                 ::aurum::RspGetValue       *response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status GetValueCommand::execute()
{
    LOGI("GetValue --------------- ");

    ::aurum::ParamType param_type = mRequest->type();
    if (param_type == ::aurum::STRING) {
        ObjectMapper *mObjMap = ObjectMapper::getInstance();
        std::shared_ptr<UiObject> obj =
            mObjMap->getElement(mRequest->elementid());

        if (obj) {
            obj->updateName();
            std::string text = obj->getText();
            mResponse->set_type(::aurum::STRING);
            mResponse->set_stringvalue(text.c_str());
            mResponse->set_status(::aurum::RspStatus::OK);
        } else {
            mResponse->set_status(::aurum::RspStatus::ERROR);
        }
    } else if (param_type == ::aurum::INT) {
        LOGI("Integer is not supported.");
        mResponse->set_status(::aurum::RspStatus::ERROR);
    } else if (param_type == ::aurum::DOUBLE) {
        ObjectMapper *mObjMap = ObjectMapper::getInstance();
        std::shared_ptr<UiObject> obj =
            mObjMap->getElement(mRequest->elementid());

        if (obj) {
            obj->updateValue();
            double value = obj->getValue();
            mResponse->set_type(::aurum::DOUBLE);
            mResponse->set_doublevalue(value);
            mResponse->set_status(::aurum::RspStatus::OK);
        } else {
            mResponse->set_status(::aurum::RspStatus::ERROR);
        }
    } else if (param_type == ::aurum::BOOL) {
        LOGI("Boolean is not supported.");
        mResponse->set_status(::aurum::RspStatus::ERROR);
    }

    return grpc::Status::OK;
}
