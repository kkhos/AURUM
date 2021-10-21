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

#ifndef _A11Y_EVENT_H_
#define _A11Y_EVENT_H_

#include "bitmask.h"
#include <string>

/**
 * @class A11yEvent
 *
 * @ingroup aurum
 *
 * @brief Enum class for A11y Event.
 */
enum class A11yEvent {
     EVENT_NONE = 0x00000,
     EVENT_WINDOW_CREATE = 0x00001,
     EVENT_WINDOW_CLOSE = 0x00002,
     EVENT_WINDOW_MINIMIZE = 0x00004,
     EVENT_WINDOW_MAXIMIZE = 0x00008,
     EVENT_WINDOW_RESTORE = 0x00010,
     EVENT_WINDOW_ACTIVATE = 0x00020,
     EVENT_WINDOW_DEACTIVATE = 0x00040,
     EVENT_WINDOW_RAISE = 0x00080,
     EVENT_WINDOW_LOWER = 0x00100,
     EVENT_WINDOW_MOVE = 0x00200,
     EVENT_WINDOW_RESIZE = 0x00400,

     EVENT_TEXT_CHANGED_INSERT = 0x00800,
     EVENT_TEXT_CHANGED_DELETE = 0x01000,

     EVENT_STATE_CHANGED_VISIBLE = 0x02000,
     EVENT_STATE_CHANGED_FOCUSED = 0x04000,
     EVENT_STATE_CHANGED_CHECKED = 0x08000,
     EVENT_STATE_CHANGED_PRESSED = 0x10000,
     EVENT_STATE_CHANGED_SELECTED = 0x20000,
};

enableEnumClassBitfield(A11yEvent);

/**
 * @class A11yEventInfo
 *
 * @ingroup aurum
 *
 * @brief Class that defines the methods for matching the actual A11y Event with an enum value.
 */
class A11yEventInfo {
public:
    /**
     * @brief A11yEventInfo constructor.
     *
     * @since_tizen 5.5
     */
    A11yEventInfo();

    /**
     * @brief A11yEventInfo constructor with event as enum value, name, pkg.
     *
     * @since_tizen 5.5
     */
    A11yEventInfo(A11yEvent event, std::string name = "", std::string pkg = "");

    /**
     * @brief A11yEventInfo constructor with event as string value, name, pkg.
     *
     * @since_tizen 5.5
     */
    A11yEventInfo(std::string event, std::string name = "", std::string pkg = "");

    /**
     * @brief A11yEventInfo destructor.
     *
     * @since_tizen 5.5
     */
    ~A11yEventInfo();

public:
    /**
     * @brief Gets A11yEvent.
     *
     * @return @A11yEvent
     *
     * @since_tizen 5.5
     */
    A11yEvent getEvent();

    /**
     * @brief Gets matched A11yEvent.
     *
     * @param event name of event
     *
     * @return @A11yEvent
     *
     * @since_tizen 5.5
     */
    A11yEvent getEvent(std::string event);

    /**
     * @brief Gets event name of event.
     *
     * @return string
     *
     * @since_tizen 5.5
     */
    std::string getName();

    /**
     * @brief Gets package name of event.
     *
     * @return string
     *
     * @since_tizen 5.5
     */
    std::string getPkg();

protected:
    const A11yEvent mEvent;
    const std::string mName;
    const std::string mPkg;
};

#endif
