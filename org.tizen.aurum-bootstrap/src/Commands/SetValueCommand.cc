/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd All Rights Reserved
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
#include "SetValueCommand.h"

SetValueCommand::SetValueCommand(const ::aurum::ReqSetValue* request,
                                 ::aurum::RspSetValue*       response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status SetValueCommand::execute()
{
    bool ret = false;
    LOGI("SetValue --------------- ");
    LOGI("text:%s", mRequest->stringvalue().c_str());

    ObjectMapper *mObjMap = ObjectMapper::getInstance();
    std::shared_ptr<UiObject> obj = mObjMap->getElement(mRequest->elementid());
    if (obj) ret = obj->setText(const_cast<std::string&>(mRequest->stringvalue()));

    if (ret) mResponse->set_status(::aurum::RspStatus::OK);
    else mResponse->set_status(::aurum::RspStatus::ERROR);

    return grpc::Status::OK;
}
