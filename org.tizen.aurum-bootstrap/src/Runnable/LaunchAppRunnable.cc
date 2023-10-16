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

#include "Aurum.h"
#include "bootstrap.h"
#include "LaunchAppCommand.h"
#include "LaunchAppRunnable.h"
#include <app_control.h>

LaunchAppRunnable::LaunchAppRunnable(std::string pkg, const google::protobuf::RepeatedPtrField<aurum::LaunchData>& data)
    : mPkg{pkg}, mData(data)
{
}

void LaunchAppRunnable::run() const
{
    app_control_h appControl;
    std::string   packageName = mPkg;
    int           ret = -1;

    if (packageName.empty()) return;

    ret = app_control_create(&appControl);
    if (ret) {
        LOGE("Launch Failed(app_control_create) Err Code : %ull", ret);
        return;
    }

    std::for_each(mData.begin(), mData.end(), [&](auto data){
        ret = app_control_add_extra_data(appControl, data.key().c_str(),data.value().c_str());
        if (ret) {
            LOGE("Launch Failed(app_control_add_extra_data) Err Code : %ull", ret);
            app_control_destroy(appControl);
            return;
        }
    });

    ret = app_control_set_app_id(appControl, packageName.c_str());
    if (ret) {
        LOGE("Launch Failed(app_control_set_app_id) Err Code : %ull", ret);
        app_control_destroy(appControl);
        return;
    }

    ret = app_control_send_launch_request(appControl, NULL, NULL);
    if (ret) {
        LOGE("Launch Failed(app_control_send_launch_request) Err Code : %ull", ret);
        app_control_destroy(appControl);
        return;
    }

    app_control_destroy(appControl);
}

std::string LaunchAppRunnable::getPkgName()
{
    return mPkg;
}
