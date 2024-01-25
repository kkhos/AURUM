/*
 * Copyright (c) 2023 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef _ACCESSIBLE_APPMANAGER_H_
#define _ACCESSIBLE_APPMANAGER_H_

#include "AccessibleApplication.h"
#include <vector>

#include "config.h"

namespace Aurum {

/**
 * @class AccessibleAppManager
 *
 * @ingroup aurum
 *
 * @brief Class that manages a list of application classes
 */
class AccessibleAppManager {
public:
    /**
     * @brief AccessibleAppManager constructor.
     *
     * @since_tizen 6.5
     */
    AccessibleAppManager();

    /**
     * @brief AccessibleAppManager destructor.
     *
     * @since_tizen 6.5
     */
    virtual ~AccessibleAppManager();

    /**
     * @brief Gets @AccessibleAppManager instance.
     *
     * @since_tizen 6.5
     */
    static AccessibleAppManager *getInstance(AccessibleAppManager *appManagerImpl = nullptr);

public:
    /**
     * @brief Gets current running application count in device.
     *
     * @return application count
     *
     * @since_tizen 6.5
     */
    virtual int getApplicationCount(void) const = 0;

    /**
     * @brief Gets application class at specific index.
     *
     * @param[in] index application index
     *
     * @return @AccessibleApplication ptr
     *
     * @since_tizen 6.5
     */
    virtual std::shared_ptr<AccessibleApplication> getApplicationAt(int index) const = 0;

    /**
     * @brief Gets application node vector.
     *
     * @return @AccessibleApplication vector
     *
     * @since_tizen 6.5
     */
    virtual std::vector<std::shared_ptr<AccessibleApplication>> getApplications(void) const = 0;

    /**
     * @brief Gets active application vector.
     *
     * @return @AccessibleApplication vector
     *
     * @since_tizen 6.5
     */
    virtual std::vector<std::shared_ptr<AccessibleApplication>> getActiveApplications(void) const;
};

}

#endif
