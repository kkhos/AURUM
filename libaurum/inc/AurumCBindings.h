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

#ifndef AURUM_C_BINDINGS_H
#define AURUM_C_BINDINGS_H

/**
 * Aurum C APIs for Rust bindings. Do not use these directly from C++ applications. Use Aurum API instead.
 */
#ifdef __cplusplus
extern "C" {
#endif
    // Initialize the Aurum library.
    void aurum_init();
    // Shutdown the Aurum library. This will release all resources.
    void aurum_shutdown();
    // Dump current screen as JSON string.
    const char* aurum_dump_screen();
    // Dump current screen as JSON string.
    // @param includeHidden If non-zero, include hidden accessibility objects.
    const char* aurum_dump_screen_ex(int includeHidden);
    // Free the allocated string memory.
    void aurum_free_string_result(const char* ptr);
    // Click the given coordinates. Returns 1 if successful. Otherwise -1.
    int aurum_click(int x, int y);
    // Press the given key code. Returns 1 if successful. Otherwise -1.
    int aurum_press_key_code(const char* key_code);
#ifdef __cplusplus
}
#endif

#endif // AURUM_C_BINDINGS_H