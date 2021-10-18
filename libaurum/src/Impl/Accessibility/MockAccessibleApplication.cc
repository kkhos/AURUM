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

#include "MockAccessibleApplication.h"
#include "MockAccessibleWindow.h"
#include "MockAccessibleNode.h"

#include <algorithm>

using namespace Aurum;

MockAccessibleApplication::MockAccessibleApplication(std::shared_ptr<AccessibleNode> node)
: AccessibleApplication(node), mWindowList{}
{
}

MockAccessibleApplication::~MockAccessibleApplication()
{
}

std::vector<std::shared_ptr<AccessibleWindow>> MockAccessibleApplication::getWindows(void)
{
    return mWindowList;
}

std::string MockAccessibleApplication::getPackageName(void)
{
    return "This.Is.Mock.Application";
}

void MockAccessibleApplication::addWindow(std::shared_ptr<AccessibleWindow> window)
{
    mWindowList.push_back(window);
}
void MockAccessibleApplication::clearWindows(void)
{
    mWindowList.clear();
}

std::shared_ptr<MockAccessibleWindow> MockAccessibleApplication::addWindow(std::string title, std::string widget, Rect<int> geometry, int properties)
{
    auto node = std::make_shared<MockAccessibleNode>(nullptr, title, title, "window", "res",widget,"style", "", geometry, 0, properties);
    auto win = std::make_shared<MockAccessibleWindow>(shared_from_this(), node);
    this->addWindow(win);
    return win;
}

