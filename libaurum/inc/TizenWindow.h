/*
 * Copyright (c) 2022 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef _TIZEN_WINDOW_H_
#define _TIZEN_WINDOW_H_

#include <gio/gio.h>

namespace Aurum {

class TizenWindow : public std::enable_shared_from_this<TizenWindow> {
public:
    TizenWindow(int pid, Rect<int> geometry, bool transformed, bool alpha, int opaque, int visibility, bool focused, bool mapped, int layer, std::string name);
    ~TizenWindow();

    int getPid();
    Rect<int> getWindowGeometry();
    bool isTransformed();
    bool isAlpah();
    int getOpaque();
    int getVisibility();
    bool isFocused();
    bool isMapped();
    std::string getName();
private:
    int mPid;                   // window's PID
    Rect<int> mWindowGeometry;  // window's geometry
    bool mIsTransformed;        // true : transformed window
    bool mIsAlpha;              // true : alpha
    int mOpaque;                // true : opaque state set window
    int mVisibility;            // 0  : visible
                                // 1  : partial visible
                                // 2  : hidden
                                // -1 : unknown
    bool mIsFocused;            // true : has focus
    bool mIsMapped;             // true : mapped
    int mLayer;                 // 200       : normal layer
                                // 250       : above layer
                                // 650 ~ 800 : notification layer
    std::string mName;          // window name
};

}

#endif
