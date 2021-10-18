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

#include "MockAccessibleWindow.h"
#include "MockAccessibleApplication.h"
#include "MockAccessibleNode.h"

using namespace Aurum;
using namespace AurumInternal;

MockAccessibleWindow::MockAccessibleWindow(std::shared_ptr<MockAccessibleApplication> app, std::shared_ptr<MockAccessibleNode> node)
: AccessibleWindow(app, node), mMockNode{node}
{
}

MockAccessibleWindow::~MockAccessibleWindow()
{
}

std::shared_ptr<MockAccessibleNode> MockAccessibleWindow::addNode(std::string text, std::string pkg, std::string role, std::string id, std::string type, std::string style, std::string automationId, Rect<int> geometry, int ifaces, int properties)
{
    auto node = std::make_shared<MockAccessibleNode>(mMockNode, text, pkg, role, id, type, style, automationId, geometry, ifaces, properties);
    mMockNode->addChild(node);
    return node;
}