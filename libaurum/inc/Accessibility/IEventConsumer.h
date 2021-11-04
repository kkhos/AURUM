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

#ifndef _IEVENT_CONSUMER_H_
#define _IEVENT_CONSUMER_H_

namespace Aurum {

/**
 * @class EventType
 *
 * @ingroup aurum
 *
 * @brief Enum class for Event Type.
 */
enum class EventType {
    none = 0,
    Object = 1,
};

/**
 * @class ObjectEventType
 *
 * @ingroup aurum
 *
 * @brief Enum class for Object Event Type.
 */
enum class ObjectEventType {
    none = 0,
    ObjectStateDefunct = 1,
};

/**
 * @class IEventConsumer
 *
 * @ingroup aurum
 *
 * @brief Class for Event generate.
 */
class IEventConsumer
{
public:
    /**
     * @brief IEventConsumer destructor.
     *
     * @since_tizen 6.5
     */
    virtual ~IEventConsumer() {};

    /**
     * @brief Notifies event to source node.
     *
     * @param[in] type1 @EventType
     * @param[in] type2 @ObjectEventType
     * @param[in] src source Node ptr
     *
     * @since_tizen 6.5
     */
    virtual void notify(int type1, int type2, void *src) = 0;
};

}

#endif
