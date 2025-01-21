/*
 * Copyright (c) 2025 Samsung Electronics Co., Ltd All Rights Reserved
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

#include "AurumCBindings.h"
#include "Aurum.h"
#include <cstring>

using namespace Aurum;

namespace
{
    constexpr const char WINDOWS_KEY[] = "\"windows\"";
}

void aurum_init()
{
    LOGI("aurum_init");
    AccessibleWatcher::getInstance();
}

const char* dump_screen()
{
    LOGI("dump_screen");
    auto device = UiDevice::getInstance();
    auto windowRoots = device->getWindowRoot();
    std::string dumpResult{WINDOWS_KEY};
    dumpResult += " : [";

    for (const auto& root: windowRoots) {
        dumpResult += root->dumpTree();
        dumpResult += ",";
    }

    // remove last comma if exists.
    if(dumpResult.back() == ',') {
        dumpResult.pop_back();
    }

    dumpResult += "]";
    char* cResult = new char[dumpResult.size() + 1]; // Consumers must free this memory later
    std::strcpy(cResult, dumpResult.c_str());
    return cResult;

}

void free_string_result(const char* ptr)
{
    LOGI("free_string_result");
    delete[] ptr;
}