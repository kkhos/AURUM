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

#include "Aurum.h"

using namespace Aurum;

SendKeyRunnable::SendKeyRunnable(std::string keycode)
    : mKeycode{keycode}
{
}

void SendKeyRunnable::run() const
{
    LOGI("sendKey %s",mKeycode.c_str());
    std::shared_ptr<UiDevice> mDevice = UiDevice::getInstance();
    mDevice->pressKeyCode(mKeycode.c_str(), KeyRequestType::PRESS);
    mDevice->pressKeyCode(mKeycode.c_str(), KeyRequestType::RELEASE);
}

std::string SendKeyRunnable::getPkg() const
{
    return std::string();
}
