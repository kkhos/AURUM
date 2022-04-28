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

#ifndef _SCREEN_ANALYZER_WATCHER_H_
#define _SCREEN_ANALYZER_WATCHER_H_

#include <mosquitto.h>

namespace Aurum {

class ScreenAnalyzerWatcher {
public:
    ScreenAnalyzerWatcher();
    ~ScreenAnalyzerWatcher();
    static void onMessage(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg);
    void PublishData(std::string path, const Size2D<int> screenSize);

private:
    const std::string serverAddress = "10.113.16.21";
    struct mosquitto *mosq;
    std::string jsontext;
    static bool doneLoad;
};

}

#endif
