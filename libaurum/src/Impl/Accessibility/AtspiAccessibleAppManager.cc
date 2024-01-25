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

#include "AtspiAccessibleAppManager.h"
#include "AtspiAccessibleApplication.h"
#include "AtspiWrapper.h"
#include "AtspiAccessibleNode.h"

using namespace Aurum;
using namespace AurumInternal;

AtspiAccessibleAppManager::AtspiAccessibleAppManager()
{
}

AtspiAccessibleAppManager::~AtspiAccessibleAppManager()
{
}

int AtspiAccessibleAppManager::getApplicationCount(void) const
{
    AtspiAccessible *root = AtspiWrapper::Atspi_get_desktop(0);
    int nchild = AtspiWrapper::Atspi_accessible_get_child_count(root, NULL);
    g_object_unref(root);

    if (nchild <= 0) return 0;
    return nchild;
}

std::shared_ptr<AccessibleApplication> AtspiAccessibleAppManager::getApplicationAt(int index) const
{
    AtspiAccessible *root = AtspiWrapper::Atspi_get_desktop(0);
    AtspiAccessible *child = AtspiWrapper::Atspi_accessible_get_child_at_index(root, index, NULL);
    g_object_unref(root);
    return std::make_shared<AtspiAccessibleApplication>(std::make_shared<AtspiAccessibleNode>(child));
}

std::vector<std::shared_ptr<AccessibleApplication>> AtspiAccessibleAppManager::getApplications(void) const
{
    std::vector<std::shared_ptr<AccessibleApplication>> ret{};
    AtspiAccessible *root = AtspiWrapper::Atspi_get_desktop(0);
    GArray *children = AtspiWrapper::Atspi_accessible_get_children(root, NULL);
    if (children) {
        ret.reserve(children->len);
        AtspiAccessible *child = nullptr;
        for (unsigned int i = 0; i < children->len; i++) {
            child = g_array_index(children, AtspiAccessible *, i);
            if (child) {
                ret.push_back(std::make_shared<AtspiAccessibleApplication>(std::make_shared<AtspiAccessibleNode>(child)));
            }
        }
        g_array_free(children, true);
    }
    g_object_unref(root);

    return ret;
}
