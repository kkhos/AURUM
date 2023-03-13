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

namespace Aurum {

/**
 * @class Sel
 *
 * @ingroup aurum
 *
 * @brief Helper class for use @UiSelector easily.
 *        This class using in Aurum feature UTC in cpp test code.
 */
class Sel {
public:
    /**
     * @brief Sets the search criteria to match the object's text.
     *
     * @param[in] text object text
     *
     * @return @UiSelector
     *
     * @since_tizen 6.5
     */
    static std::shared_ptr<UiSelector> text(std::string text);

    /**
     * @brief Sets the search criteria to match the object's type.
     *
     * @param[in] type object type
     *
     * @return @UiSelector
     *
     * @since_tizen 6.5
     */
    static std::shared_ptr<UiSelector> type(std::string type);

    /**
     * @brief Sets the search criteria to match the object's style.
     *
     * @param[in] style object style
     *
     * @return @UiSelector
     *
     * @since_tizen 6.5
     */
    static std::shared_ptr<UiSelector> style(std::string style);

    /**
     * @brief Sets the depth of the selector.
     *
     * @param[in] depth tree depth from root
     *
     * @return @UiSelector
     *
     * @since_tizen 6.5
     */
    static std::shared_ptr<UiSelector> depth(int depth);
};

}

#endif
