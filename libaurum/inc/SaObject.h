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

#ifndef _SA_OBJECT_H_
#define _SA_OBJECT_H_

#include "config.h"
#include <vector>

namespace Aurum {

/**
 * @class SaObject
 *
 * @ingroup aurum
 *
 * @brief A SaObject is a result of Screen Analyzer.
 */
class SaObject : public IObject, public std::enable_shared_from_this<SaObject> {
public:

    /**
     * @brief SaObject constructor with initial values.
     *
     * @since_tizen 7.0
     */
    SaObject(std::string id, std::string type, Rect<int> geometry, std::string ocrText, std::vector<std::string> states);

    /**
     * @brief SaObject destructor.
     *
     * @since_tizen 7.0
     */
    ~SaObject();

    /**
     * @copydoc UiObject::getId()
     */
    std::string getId() const override;

    /**
     * @copydoc UiObject::getType()
     */
    std::string getType() const override;

    /**
     * @copydoc UiObject::getScreenBoundingBox()
     */
    const Rect<int> getScreenBoundingBox() const override;

    std::string getOcrText() const override;

    /**
     * @copydoc UiObject::isFocusable()
     */
    bool isFocusable() const override;

    /**
     * @copydoc UiObject::isFocused()
     */
    bool isFocused() const override;

    /**
     * @copydoc UiObject::isClickable()
     */
    bool isClickable() const override;

    /**
     * @copydoc UiObject::isActive()
     */
    bool isActive() const override;

    /**
     * @copydoc UiObject::isShowing()
     */
    bool isShowing() const override;

private:
    std::string mId;
    std::string mElementType;
    Rect<int> mScreenBoundingBox;
    std::string mOcrText;
    bool mIsFocusable;
    bool mIsFocused;
    bool mIsClickable;
    bool mIsActive;
};

}

#endif
