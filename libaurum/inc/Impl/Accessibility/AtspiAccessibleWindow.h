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

#ifndef _ATSPI_ACCESSIBLE_WINDOW_H_
#define _ATSPI_ACCESSIBLE_WINDOW_H_

#include "AccessibleWindow.h"

/**
 * @internal
 *
 * @class AtspiAccessibleWindow
 *
 * @ingroup aurum
 *
 * @brief Class that matches atspi Window Node.
 */
class AtspiAccessibleWindow : public AccessibleWindow {
public:
    /**
     * @brief AtspiAccessibleWindow constructor.
     *
     * @param[in] app @AccessibleApplication
     * @param[in] node @AccessibleNode
     *
     * @since_tizen 5.5
     */
    AtspiAccessibleWindow(std::shared_ptr<AccessibleApplication> app, std::shared_ptr<AccessibleNode> node);

    /**
     * @brief AtspiAccessibleWindow desturctor.
     *
     * @since_tizen 5.5
     */
    ~AtspiAccessibleWindow();

};

#endif
