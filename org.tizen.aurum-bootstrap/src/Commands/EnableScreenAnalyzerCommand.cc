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
#include "EnableScreenAnalyzerCommand.h"
#include "UiDevice.h"

EnableScreenAnalyzerCommand::EnableScreenAnalyzerCommand(const ::aurum::ReqEnableScreenAnalyzer *request,
                                     ::aurum::RspEnableScreenAnalyzer *response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status EnableScreenAnalyzerCommand::execute()
{
    LOGI("EnableScreenAnalyzer test --------------- ");

    LOGI("Screen Analyzer enabled (%d) Server IP : (%s)", mRequest->enable(), mRequest->serverip().c_str());
    std::shared_ptr<UiDevice> mDevice = UiDevice::getInstance();
    mDevice->setWithScreenAnalyzer(mRequest->enable());
#ifdef MQTT_ENABLED
    if (mRequest->serverip().size() > 0)
        mDevice->getSAWatcher()->SetServerIp(mRequest->serverip());
#endif

    return grpc::Status::OK;
}
