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
#include "GoToKeyCommand.h"
#include "UiDevice.h"
#include "UiObject.h"
#include "UiObject.h"
#include "UiDevice.h"
#include "UiSelector.h"
#include "Sel.h"

GoToKeyCommand::GoToKeyCommand(const ::aurum::ReqGoToKey *request,
                                     ::aurum::RspGoToKey *response)
    : mRequest{request}, mResponse{response}
{
}

std::shared_ptr<UiObject> GoToKeyCommand::searchFocusedObject(void)
{
    std::shared_ptr<UiDevice> mDevice = UiDevice::getInstance();

    auto sel = std::make_shared<UiSelector>();
    sel->isFocused(true);

    return mDevice->findObject(sel);

}

::grpc::Status GoToKeyCommand::execute()
{
    LOGI("GoToKey --------------- ");

    std::shared_ptr<UiDevice> mDevice = UiDevice::getInstance();
    ObjectMapper *mObjMap = ObjectMapper::getInstance();
    std::shared_ptr<UiObject> obj = mObjMap->getElement(mRequest->elementid());

    obj->updateExtents();
    LOGI("fobj (%d %d %d %d)", obj->getScreenBoundingBox().mTopLeft.x, obj->getScreenBoundingBox().mTopLeft.y, obj->getScreenBoundingBox().width(), obj->getScreenBoundingBox().height());
    if (obj) {
        LOGI("obj exist in map");

        while(1)
        {
            auto fobj = searchFocusedObject();
            fobj->updateExtents();
            LOGI("fobj (%d %d %d %d)", fobj->getScreenBoundingBox().mTopLeft.x, fobj->getScreenBoundingBox().mTopLeft.y, fobj->getScreenBoundingBox().width(), fobj->getScreenBoundingBox().height());
            if (fobj->getScreenBoundingBox().isInRect(obj->getScreenBoundingBox().midPoint()))
            {
                break;
            }

            //Need to implement more
            mDevice->pressKeyCode("Right", KeyRequestType::STROKE);
        }
    }

    return grpc::Status::OK;
}
