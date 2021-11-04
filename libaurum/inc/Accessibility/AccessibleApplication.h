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

#ifndef _ACCESSIBLE_APPLICATION_H_
#define _ACCESSIBLE_APPLICATION_H_

#include "AccessibleWindow.h"
#include "AccessibleNode.h"

#include <memory>

namespace Aurum {

/**
 * @class AccessibleApplication
 *
 * @ingroup aurum
 *
 * @brief Class that matches atspi Application Node.
 *        It provides information such as active window, application name.
 */
class AccessibleApplication {
public:
    /**
     * @brief AccessibleApplication constructor.
     *
     * @since_tizen 6.5
     */
    AccessibleApplication(std::shared_ptr<AccessibleNode> node);

    /**
     * @brief AccessibleApplication destructor.
     *
     * @since_tizen 6.5
     */
    virtual ~AccessibleApplication();

public:
    /**
     * @brief Gets matched @AccessibleNode.
     *
     * @return @AccessibleNode
     *
     * @since_tizen 6.5
     */
    std::shared_ptr<AccessibleNode> getAccessibleNode();

    /**
     * @brief Checks this active or not.
     *
     * @return true if it actives, else false
     *
     * @since_tizen 6.5
     */
    bool isActive(void);

public:
    /**
     * @brief Gets application's window.
     *
     * @return @AccessibleWindow vector
     *
     * @since_tizen 6.5
     */
    virtual std::vector<std::shared_ptr<AccessibleWindow>> getWindows(void) = 0;

    /**
     * @brief Gets currently active window.
     *
     * @return @AccessibleWindow vector
     *
     * @since_tizen 6.5
     */
    virtual std::vector<std::shared_ptr<AccessibleWindow>> getActiveWindows(void);

    /**
     * @brief Gets application package name.
     *
     * @return string
     *
     * @since_tizen 6.5
     */
    virtual std::string getPackageName(void) = 0;

private:
    std::shared_ptr<AccessibleNode> mNode;
};

}

#endif
