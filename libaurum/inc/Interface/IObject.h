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

#ifndef _IOBJECT_H_
#define _IOBJECT_H_

#include "config.h"
#include "Rect.h"
#include <string>

namespace Aurum {


class IObject {
public:

    virtual ~IObject() {}

    virtual std::string getId() const = 0;

    virtual std::string getType() const = 0;

    virtual Rect<int> getScreenBoundingBox() const = 0;

    virtual std::string getOcrText() const = 0;
    
    /**
     * @copydoc UiObject::isFocusable()
     */
    virtual bool isFocusable() const = 0;

    /**
     * @copydoc UiObject::isFocused()
     */
    virtual bool isFocused() const = 0;

    /**
     * @copydoc UiObject::isClickable()
     */
    virtual bool isClickable() const = 0;

    /**
     * @copydoc UiObject::isActive()
     */
    virtual bool isActive() const = 0;

    /**
     * @copydoc UiObject::isShowing()
     */
    virtual bool isShowing() const = 0;
};

}

#endif
