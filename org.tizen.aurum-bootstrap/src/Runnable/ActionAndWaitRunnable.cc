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
#include "bootstrap.h"
#include "ActionAndWaitEventCommand.h"
#include "ActionAndWaitEventRunnable.h"
#include <app_control.h>

ActionAndWaitEventRunnable::ActionAndWaitEventRunnable(int type, std::string elementId, std::string keyCode)
    : mType{type}, mElementId(elementId), mKeyCode(keycode)
{
}

void ActionAndWaitEventRunnable::run() const
{
    //Do click or send key
}

int ActionAndWaitEventRunnable::getType()
{
    return mType;
}

std::string ActionAndWaitEventRunnable::getElmentId()
{
    return mElementId;
}

std::string ActionAndWaitEventRunnable::getKeyCode()
{
    return mKeyCode;
}

int ActionAndWaitEventRunnable::getEventType()
{
    return mEventType;
}

int ActionAndWaitEventRunnable::getTimeoutMs()
{
    return mTimeoutMs;
}