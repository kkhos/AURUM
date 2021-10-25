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

#ifndef _ATSPI_ACCESSIBLE_APPLICATION_H_
#define _ATSPI_ACCESSIBLE_APPLICATION_H_

#include "AccessibleApplication.h"

#include <memory>
#include <atspi/atspi.h>

/**
 * @internal
 *
 * @class AtspiAccessibleApplication
 *
 * @ingroup aurum
 *
 * @brief Class that matches atspi Application Node.
 *        It provides information such as active window, application name.
 */
class AtspiAccessibleApplication : public AccessibleApplication, public std::enable_shared_from_this<AtspiAccessibleApplication> {
public:
    /**
     * @brief AtspiAccessibleApplication constructor.
     *
     * @param[in] node @AccessibleNode
     *
     * @since_tizen 6.0
     */
    AtspiAccessibleApplication(std::shared_ptr<AccessibleNode> node);

    /**
     * @brief AtspiAccessibleApplication destructor.
     *
     * @since_tizen 6.0
     */
    virtual ~AtspiAccessibleApplication();

public:
    /**
     * @copydoc @AccessibleApplication::getWindows()
     */
    std::vector<std::shared_ptr<AccessibleWindow>> getWindows(void) override;

    /**
     * @copydoc @AccessibleApplication::getPackageName()
     */
    std::string getPackageName(void) override;
};

#endif
