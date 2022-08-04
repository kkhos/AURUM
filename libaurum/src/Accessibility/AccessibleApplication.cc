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

#include "Aurum.h"

#include <algorithm>

using namespace Aurum;

AccessibleApplication::AccessibleApplication(std::shared_ptr<AccessibleNode> node)
: mNode{node}
{
}

AccessibleApplication::~AccessibleApplication()
{
}

std::shared_ptr<AccessibleNode> AccessibleApplication::getAccessibleNode()
{
    return mNode;
}

bool AccessibleApplication::isActive(void)
{
    auto children = this->getActiveWindows();
    return children.size() > 0;
}

std::vector<std::shared_ptr<AccessibleWindow>> AccessibleApplication::getActiveWindows(void)
{
    auto children = getWindows();

    children.erase(std::remove_if(children.begin(), children.end(), [](auto child){
                        return !(child->isActive() && child->isShowing() && (child->getAccessibleNode()->getChildCount() > 0));
                    }), children.end());

    return children;
}
