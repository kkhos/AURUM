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

#ifndef _SEL_H_
#define _SEL_H_

#include "config.h"

#include <memory>
#include <string>

#include "UiSelector.h"

/**
 * @brief Sel class
 * @since_tizen 5.5
 */
class Sel {
public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    static std::shared_ptr<UiSelector> text(std::string text);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    static std::shared_ptr<UiSelector> type(std::string text);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    static std::shared_ptr<UiSelector> style(std::string text);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    static std::shared_ptr<UiSelector> depth(int depth);
};

#endif
