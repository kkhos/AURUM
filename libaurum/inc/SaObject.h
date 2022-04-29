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

#ifndef _SA_OBJECT_H_
#define _SA_OBJECT_H_

#include "config.h"
#include <vector>

namespace Aurum {

class SaObject : public std::enable_shared_from_this<SaObject> {
public:
    SaObject(std::string id, std::string type, Rect<int> geometry, std::string ocrText, std::vector<std::string> states);
    ~SaObject();

    std::string getId();
    std::string getElementType();
    const Rect<int> getScreenBoundingBox();
    std::string getOcrText();
    bool isFocusable();
    bool isFocused();
    bool isClickable();

private:
    std::string mId;
    std::string mElementType;
    Rect<int> mScreenBoundingBox;
    std::string mOcrText;
    bool mIsFocusable;
    bool mIsFocused;
    bool mIsClickable;
};

}

#endif
