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

#include "GetIncludeHiddenCommand.h"
#include "UiObject.h"

GetIncludeHiddenCommand::GetIncludeHiddenCommand(const ::aurum::ReqGetIncludeHidden *request,
                               ::aurum::RspGetIncludeHidden *response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status GetIncludeHiddenCommand::execute()
{
    LOGI("GetIncludeHidden--------------- ");


    ObjectMapper *mObjMap = ObjectMapper::getInstance();
    std::shared_ptr<UiObject> obj = mObjMap->getElement(mRequest->elementid());
    if (obj) {
        mResponse->set_enabled(obj->getIncludeHidden());
        mResponse->set_status(::aurum::RspStatus::OK);    
    } else {
        mResponse->set_status(::aurum::RspStatus::ERROR);
    }

    return grpc::Status::OK;
}
