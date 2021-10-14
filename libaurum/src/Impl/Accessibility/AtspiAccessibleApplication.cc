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

#include "AtspiAccessibleApplication.h"
#include "AtspiAccessibleWindow.h"
#include "AtspiWrapper.h"

#include <algorithm>
#include <vector>

AtspiAccessibleApplication::AtspiAccessibleApplication(std::shared_ptr<AccessibleNode> node)
: AccessibleApplication(node)
{
}

AtspiAccessibleApplication::~AtspiAccessibleApplication()
{
}

std::vector<std::shared_ptr<AccessibleWindow>> AtspiAccessibleApplication::getWindows(void)
{
    std::vector<std::shared_ptr<AccessibleWindow>> ret{};
    auto children = getAccessibleNode()->getChildren();

    std::transform(children.begin(), children.end(), std::back_inserter(ret),
        [&](auto child) {
            return std::make_shared<AtspiAccessibleWindow>(this->shared_from_this(), child);
        }
    );

    return ret;
}

std::string AtspiAccessibleApplication::getPackageName(void)
{
    return getAccessibleNode()->getText();
}