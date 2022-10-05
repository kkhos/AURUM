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

#include "Aurum.h"
#include "bootstrap.h"
#include "ActionAndWaitEventCommand.h"
#include "ActionAndWaitEventRunnable.h"
#include <app_control.h>

ActionAndWaitEventRunnable::ActionAndWaitEventRunnable(int type, std::string elementId, std::string keyCode)
    : mType{type}, mElementId(elementId), mKeyCode(keyCode)
{
}

void ActionAndWaitEventRunnable::run() const
{
/**
 *
 *  enum ActionType{
 *    CLICK = 0;
 *    KEY = 1;
 *    FOCUS = 2;
 *  }
 *
 **/
    if (mType == 0) {
        ObjectMapper *mObjMap = ObjectMapper::getInstance();
        std::shared_ptr<UiObject> obj = mObjMap->getElement(mElementId);

        if (obj) {
            LOGD("Click Object Id(%s)", mElementId.c_str());
            mPkg = obj->getApplicationPackage();
            obj->click();
        }
        else
            LOGE("Object Id(%s) is invalid or not exist on view", mElementId.c_str());
    }
    else if (mType == 1) {
        LOGD("Send KeyEvent(%s)", mKeyCode.c_str());
        std::shared_ptr<UiDevice> mDevice = UiDevice::getInstance();
        mDevice->pressKeyCode(mKeyCode, KeyRequestType::STROKE);
    }
    else if (mType == 2) {
        ObjectMapper *mObjMap = ObjectMapper::getInstance();
        std::shared_ptr<UiObject> obj = mObjMap->getElement(mElementId);
        if (obj) {
            LOGD("SetFocus Object Id(%s)", mElementId.c_str());
            mPkg = obj->getApplicationPackage();
            obj->setFocus();
        }
        else
            LOGE("Object Id(%s) is invalid or not exist on view", mElementId.c_str());
    }
}

std::string ActionAndWaitEventRunnable::getPkg() const
{
    return mPkg;
}
