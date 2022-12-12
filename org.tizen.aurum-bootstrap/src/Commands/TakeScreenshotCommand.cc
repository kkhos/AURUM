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
#include "TakeScreenshotCommand.h"
#include <fstream>
#include "UiObject.h"
#include "UiDevice.h"

TakeScreenshotCommand::TakeScreenshotCommand(
    const ::aurum::ReqTakeScreenshot *request,
    ::grpc::ServerWriter< ::aurum::RspTakeScreenshot> *writer)
    : mRequest{request}, mWriter{writer}
{
}

::grpc::Status TakeScreenshotCommand::execute()
{
    LOGI("TakeScreenshot --------------- ");

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
    std::shared_ptr<UiDevice> mDevice = UiDevice::getInstance();
    mDevice->takeScreenshot(path, 1.0, 1);

    std::ifstream ifs(path, std::ifstream::binary);
    ::aurum::RspTakeScreenshot rsp;
    int size = mDevice->getScreenSize().width * mDevice->getScreenSize().height;
    char *buf = (char *) calloc(size, sizeof(char));

    while (!ifs.eof()) {
        ifs.read(buf, size);
        rsp.set_image(buf, ifs.gcount());
        mWriter->Write(rsp);
    }
    ifs.close();

    return grpc::Status::OK;
}
