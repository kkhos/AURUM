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

#ifdef TIZEN
#include <Ecore.h>
static std::once_flag init_flag;
static std::once_flag shutdown_flag;
#endif

using namespace Aurum;

void aurum_init()
{
    LOGI("aurum_init");
// TODO: Move this to Application side.
#ifdef TIZEN
    std::call_once(init_flag, []() {
        ecore_init();
        std::thread([]() {
            ecore_main_loop_begin();
        }).detach();
    });
#endif
    AccessibleWatcher::getInstance();
}

void aurum_shutdown()
{
    LOGI("aurum_shutdown");
// TODO: Move this to Application side.
#ifdef TIZEN
    std::call_once(shutdown_flag, []() {
        std::thread([]() {
            ecore_main_loop_quit();
        }).detach();

        ecore_shutdown();
    });
#endif
}

const char* aurum_dump_screen()
{
    LOGI("dump_screen");
    auto device = UiDevice::getInstance();
    auto windowRoots = device->getWindowRoot();
    std::string dumpResult{"["};

    for (const auto& root: windowRoots) {
        root->setIncludeHidden(true);
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

void aurum_free_string_result(const char* ptr)
{
    LOGI("free_string_result");
    delete[] ptr;
}

int aurum_click(int x, int y)
{
    LOGI("click(%d,%d)", x, y);
    auto device = UiDevice::getInstance();
    return device->click(x, y) ? 0 : -1;;
}

int aurum_press_key_code(const char *key_code)
{
    LOGI("press_key_code(%s)", key_code);
    auto device = UiDevice::getInstance();
    return device->pressKeyCode(key_code, KeyRequestType::STROKE) ? 0 : -1;
}
