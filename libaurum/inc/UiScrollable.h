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

#ifndef _UI_SCROLLABLE_H_
#define _UI_SCROLLABLE_H_

#include "UiObject.h"
#include <memory>

namespace Aurum {

/**
 * @brief UiScrollable class
 * @since_tizen 6.5
 */
class UiScrollable : public UiObject
{
public:
    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    UiScrollable(std::shared_ptr<UiSelector> selector);
    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    UiScrollable();

public:
    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    bool exists(UiObject *obj);

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    bool scrollToObject(UiObject *obj);

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    bool scrollForward();
    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    bool scrollForward(int steps);

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    bool scrollBackward();
    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    bool scrollBackward(int steps);

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    bool scrollToBegin();

private:
    /**
     * @brief TBD
     */
    std::shared_ptr<UiSelector> mSelector;

    /**
     * @brief TBD
     */
    int mMaxSearchSwipe;

    /**
     * @brief TBD
     */
    int mScrollStep;
};

}

#endif
