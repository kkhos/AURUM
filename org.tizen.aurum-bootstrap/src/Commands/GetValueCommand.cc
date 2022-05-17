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
#include "GetValueCommand.h"
#include "UiObject.h"
#include "UiDevice.h"
#include "SaObject.h"
#include "UiSelector.h"
#include "Sel.h"
#include "ISearchable.h"

GetValueCommand::GetValueCommand(const ::aurum::ReqGetValue *request,
                                 ::aurum::RspGetValue *response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status GetValueCommand::execute()
{
    LOGI("GetValue --------------- ");

    std::shared_ptr<UiDevice> mDevice = UiDevice::getInstance();
    if (mDevice->getExternalAppLaunched()) 
    {
        LOGE("WCC Capture prepare");
        struct tm timeinfo;
        time_t now = time(0);
        if (!localtime_r(&now, &timeinfo)) {
            LOGE("fail to get localtime. Screenshot cancelled");
            return grpc::Status::CANCELLED;
        }
        char name[128];
        std::snprintf(name, 128, "/tmp/screenshot-%d-%d-%d-%d:%d:%d.png",
                                (timeinfo.tm_year + 1900), (timeinfo.tm_mon + 1), timeinfo.tm_mday,
                                timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
        std::string path(name);
        mDevice->RequestScreenAnalyze(path);

        std::vector<std::shared_ptr<SaObject>> founds = {};

        auto tempSel = std::make_shared<UiSelector>();
        tempSel->id(mRequest->elementid());
        auto selectors  = std::vector<std::shared_ptr<UiSelector>>{tempSel};

        LOGE("WCC Search Object start");
        for ( auto &sel : selectors ) {
            auto ret = mDevice->getScw()->findSaObjects(sel);
            std::move(std::begin(ret), std::end(ret), std::back_inserter(founds));
        }
        if (founds.size() > 0) {
            for (auto& found : founds) {
                SaObject *obj = found.get();
                std::string text = obj->getOcrText();
                mResponse->set_stringvalue(text.c_str());
                mResponse->set_status(::aurum::RspStatus::OK);
            }
        }
    }
    else
    {
        ObjectMapper *mObjMap = ObjectMapper::getInstance();
        std::shared_ptr<UiObject> obj = mObjMap->getElement(mRequest->elementid());

        if (obj) {
            obj->updateName();
            std::string text = obj->getText();
            mResponse->set_stringvalue(text.c_str());
            mResponse->set_status(::aurum::RspStatus::OK);
        }
    }

    return grpc::Status::OK;
}
