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

#ifndef _A11Y_EVENT_HANDLER_H_
#define _A11Y_EVENT_HANDLER_H_

#include "A11yEvent.h"
#include <functional>

namespace Aurum {

using EventHandler = std::function<bool(void *, const A11yEvent, std::shared_ptr<AccessibleNode>)>;

/**
 * @class A11yEventHandler
 *
 * @ingroup aurum
 *
 * @brief Class that handles the A11yEvent.
 */
class A11yEventHandler {
public:
    /**
    * @brief A11yEventHandler constructor with event type, callback and data
    *
    * @param[in] type @A11yEvent
    * @param[in] cb user callback
    * @param[in] data user data
    *
    * @since_tizen 7.5
    */
    A11yEventHandler(A11yEvent type, EventHandler cb, void *data);

    /**
    * @brief Gets A11yEvent
    *
    * @return @A11yEvent
    *
    * @since_tizen 7.5
    */
    A11yEvent getType() const;

    /**
    * @brief Calls user callback function.
    *
    * @param[in] pkg package name
    *
    * @return false removes user callback
    *
    * @since_tizen 7.5
    */
    bool operator() (std::shared_ptr<AccessibleNode> node);

private:
    A11yEvent mType;
    EventHandler mCb;
    void *mData;
};

}

#endif
