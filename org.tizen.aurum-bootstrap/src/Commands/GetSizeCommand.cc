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

#include "GetSizeCommand.h"
#include "UiObject.h"
#include "UiDevice.h"
#include "UiSelector.h"
#include "Sel.h"
#include "ISearchable.h"
#include "config.h"
#ifdef MQTT_ENABLED
#include "SaObject.h"
#endif


GetSizeCommand::GetSizeCommand(const ::aurum::ReqGetSize *request,
                               ::aurum::RspGetSize *response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status GetSizeCommand::execute()
{
    LOGI("GetSize --------------- ");

    std::shared_ptr<UiDevice> mDevice = UiDevice::getInstance();

#ifdef MQTT_ENABLED
    if (mDevice->getExternalAppLaunched())
    {
        mDevice->RequestScreenAnalyze();

        std::vector<std::shared_ptr<SaObject>> founds = {};

        auto tempSel = std::make_shared<UiSelector>();
        tempSel->id(mRequest->elementid());
        auto selectors  = std::vector<std::shared_ptr<UiSelector>>{tempSel};

        for ( auto &sel : selectors ) {
            auto ret = mDevice->getSAWatcher()->findSaObjects(sel);
            std::move(std::begin(ret), std::end(ret), std::back_inserter(founds));
        }
        if (founds.size() > 0) {
            for (auto& found : founds) {
                SaObject *obj = found.get();
                ::aurum::Rect *rect = mResponse->mutable_size();
                const Rect<int> &size = obj->getScreenBoundingBox();
                rect->set_x(size.mTopLeft.x);
                rect->set_y(size.mTopLeft.y);
                rect->set_width(size.width());
                rect->set_height(size.height());
            }
        }
    }
    else
#endif
    {
        ::aurum::ReqGetSize_CoordType type = mRequest->type();
        ObjectMapper *mObjMap = ObjectMapper::getInstance();
        std::shared_ptr<UiObject> obj = mObjMap->getElement(mRequest->elementid());
        if (obj) {
            obj->updateExtents();
            ::aurum::Rect *rect = mResponse->mutable_size();
            if (type == ::aurum::ReqGetSize_CoordType::ReqGetSize_CoordType_SCREEN) {
                const Rect<int> &size = obj->getScreenBoundingBox();
                rect->set_x(size.mTopLeft.x);
                rect->set_y(size.mTopLeft.y);
                rect->set_width(size.width());
                rect->set_height(size.height());
            }
            else {
                const Rect<int> &windowRelativeSize = obj->getWindowBoundingBox();
                rect->set_x(windowRelativeSize.mTopLeft.x);
                rect->set_y(windowRelativeSize.mTopLeft.y);
                rect->set_width(windowRelativeSize.width());
                rect->set_height(windowRelativeSize.height());
            }
        }
    }

    return grpc::Status::OK;
}
