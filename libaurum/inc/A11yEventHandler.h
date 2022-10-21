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

#ifndef _A11Y_EVENT_HANDLER_H_
#define _A11Y_EVENT_HANDLER_H_

#include "A11yEvent.h"

namespace Aurum {

typedef bool (*EventHandler)(void *data, const A11yEvent type, std::string pkg);

class A11yEventHandler {
public:
    A11yEventHandler(A11yEvent type, EventHandler cb, void *data);

    A11yEvent getType() const;

    bool operator() (std::string pkg);


private:
    A11yEvent mType;
    EventHandler mCb;
    void *mData;
};

}

#endif