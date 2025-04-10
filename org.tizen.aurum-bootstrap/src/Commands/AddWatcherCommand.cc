/*
 * Copyright (c) 2025 Samsung Electronics Co., Ltd All Rights Reserved
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
#include <chrono>
#include <thread>

#include "AddWatcherCommand.h"
#include "UiObject.h"
#include "UiDevice.h"

AddWatcherCommand::AddWatcherCommand(const ::aurum::ReqAddWatcher *request,
                                           ::aurum::RspAddWatcher *response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status AddWatcherCommand::execute()
{
    LOGI("AddWatcher --------------- ");
    std::shared_ptr<UiDevice> mDevice = UiDevice::getInstance();

    ::aurum::ReqAddWatcher_Type watcherType = mRequest->type();
    std::string text = mRequest->text();
    std::string pkg = mRequest->pkg();
    std::string target = mRequest->target();
    std::string keycode = mRequest->xf86keycode();
    int duration = mRequest->durationms();

    mDevice->registerCallback(A11yEvent::EVENT_WINDOW_ACTIVATE,    
                              [=](void *data, A11yEvent type, std::shared_ptr<AccessibleNode> node)->bool {
                                    std::shared_ptr<UiObject> root = std::make_shared<UiObject>(mDevice, nullptr, node);
                                    auto selector = std::make_shared<UiSelector>();
                                    if (!text.empty()) selector->text(text);
                                    if (!pkg.empty()) selector->pkg(pkg);
                                    auto found = root->findObject(selector);

                                    if (found != nullptr) {
                                        if (watcherType == ::aurum::ReqAddWatcher_Type::ReqAddWatcher_Type_TAP)
                                        {
                                            found->click();
                                        }
                                        else if (watcherType == ::aurum::ReqAddWatcher_Type::ReqAddWatcher_Type_TAP_ON)
                                        {
                                            auto selector = std::make_shared<UiSelector>();
                                            selector->text(target);
                                            LOGI("target %s", target.c_str());
                                            auto target = root->findObject(selector);
                                            if (target != nullptr) {
                                                LOGI("KHS found click");
                                                target->click();
                                            }
                                        }
                                        else if (watcherType == ::aurum::ReqAddWatcher_Type::ReqAddWatcher_Type_PRESS)
                                        {
                                            mDevice->pressKeyCode(keycode, KeyRequestType::STROKE);

                                        }
                                        else if (watcherType == ::aurum::ReqAddWatcher_Type::ReqAddWatcher_Type_SLEEP)
                                        {
                                            std::this_thread::sleep_for(std::chrono::milliseconds{duration});
                                        }
                                    }
                                    return true;
                              },
                              nullptr);

  
    mResponse->set_status(::aurum::RspStatus::OK);
    return grpc::Status::OK;
}
