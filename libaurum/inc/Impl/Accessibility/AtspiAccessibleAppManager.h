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

#ifndef _ATSPI_ACCESSIBLE_APPMANAGER_H_
#define _ATSPI_ACCESSIBLE_APPMANAGER_H_

#include "AccessibleAppManager.h"

using namespace Aurum;

namespace AurumInternal {

/**
 * @internal
 *
 * @class AtspiAccessibleAppManager
 *
 * @ingroup aurum
 *
 * @brief Class that manages a list of application classes
 */
class AtspiAccessibleAppManager : public AccessibleAppManager {
public:
    AtspiAccessibleAppManager();
    virtual ~AtspiAccessibleAppManager();

public:
    /**
     * @copydoc @AccessibleAppManager::getApplicationCount()
     */
    virtual int getApplicationCount(void) const override;

    /**
     * @copydoc @AccessibleAppManager::getApplicationAt()
     */
    virtual std::shared_ptr<AccessibleApplication> getApplicationAt(int index) const override;

    /**
     * @copydoc @AccessibleAppManager::getApplications()
     */
    virtual std::vector<std::shared_ptr<AccessibleApplication>> getApplications(void) const override;

private:
};

}

#endif
