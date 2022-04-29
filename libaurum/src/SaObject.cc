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

#include <unistd.h>
#include <utility>
#include <vector>
#include <chrono>
#include <algorithm>
#include <iostream>

#include "ScreenAnalyzerWatcher.h"

using namespace Aurum;

SaObject::SaObject(std::string id, std::string type, Rect<int> geometry, std::string ocrText, std::vector<std::string> states)
    : mId(id), mElementType(type), mScreenBoundingBox(geometry), mOcrText(ocrText)
{
    mIsFocusable = false;
    mIsFocused = false;
    mIsClickable = false;
    for (auto state : states)
    {
        if (state == "clickable") mIsClickable = true;
        else if (state == "focused") mIsFocused = true;
        else if (state == "focusable") mIsFocusable = true;
    }
}

SaObject::~SaObject()
{

}

std::string SaObject::getId()
{
    return mId;
}

std::string SaObject::getElementType()
{
    return mElementType;
}

const Rect<int> SaObject::getScreenBoundingBox()
{
    return mScreenBoundingBox;
}

std::string SaObject::getOcrText()
{
    return mOcrText;
}

bool SaObject::isFocusable()
{
    return mIsFocusable;
}

bool SaObject::isFocused()
{
    return mIsFocused;
}

bool SaObject::isClickable()
{
    return mIsClickable;
}