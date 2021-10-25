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

#ifndef _ACCESSIBLE_WINDOW_H_
#define _ACCESSIBLE_WINDOW_H_

#include "AccessibleNode.h"

#include <string>
#include <memory>

class AccessibleApplication;

/**
 * @class AccessibleWindow
 *
 * @ingroup aurum
 *
 * @brief Class that matches atspi Window Node.
 *        It provides information such as window title, window properties.
 */
class AccessibleWindow {
public:
    /**
     * @brief AccessibleWindow constructor.
     *
     * @param[in] app @AccessibleApplication
     * @param[in] node @AccessibleNode
     *
     * @since_tizen 6.0
     */
    AccessibleWindow(std::shared_ptr<AccessibleApplication> app, std::shared_ptr<AccessibleNode> node);

    /**
     * @brief AccessibleWindow destructor.
     *
     * @since_tizen 6.0
     */
    virtual ~AccessibleWindow();

public:
    /**
     * @brief Gets window title.
     *
     * @return string
     *
     * @since_tizen 6.0
     */
    std::string getTitle(void);

    /**
     * @brief Checks current window showing or not.
     *
     * @return true if it is showing, else false
     *
     * @since_tizen 6.0
     */
    bool isShowing();

    /**
     * @brief Checks current window active or not.
     *
     * @return true if it actives, else false
     *
     * @since_tizen 6.0
     */
    bool isActive();

    /**
     * @brief Gets the application node that this window belongs.
     *
     * @return @AccessibleApplication ptr
     *
     * @since_tizen 6.0
     */
    std::shared_ptr<AccessibleApplication> getApplication(void);

    /**
     * @brief Gets matched @AccessibleNode.
     *
     * @return @AccessibleNode
     *
     * @since_tizen 5.5
     */
    std::shared_ptr<AccessibleNode> getAccessibleNode(void);

private:
    std::shared_ptr<AccessibleApplication> mApp;
    std::shared_ptr<AccessibleNode> mNode;
};

#endif
