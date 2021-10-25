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

#ifndef _IEVENT_SOURCE_H_
#define _IEVENT_SOURCE_H_

#include "IEventConsumer.h"
#include <memory>

/**
 * @class IEventSource
 *
 * @ingroup aurum
 *
 * @brief Class for Event generate.
 */
class IEventSource
{
public:
    /**
     * @brief IEventSource destructor.
     *
     * @since_tizen 5.5
     */
    virtual ~IEventSource() {};

    /**
     * @brief Attaches event source.
     *
     * @param[in] source @IEventConsumer
     *
     * @since_tizen 5.5
     */
    virtual void attach(std::shared_ptr<IEventConsumer> source) = 0;

    /**
     * @brief Detaches event source.
     *
     * @param[in] source @IEventConsumer
     *
     * @since_tizen 5.5
     */
    virtual void detach(std::shared_ptr<IEventConsumer> source) = 0;

    /**
     * @brief Notifies all attached event.
     *
     * @param[in] type1 @EventType
     * @param[in] type2 @ObjectEventType
     * @param[in] src source Node ptr
     *
     * @since_tizen 5.5
     */
    virtual void notifyAll(int type1, int type2, void *src) = 0;
};

#endif
