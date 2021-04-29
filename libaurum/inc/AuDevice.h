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
#ifndef _AU_DEIVCE_H_
#define _AU_DEVICE_H_

namespace aurum
{
    class AuApp;

    class AuDevice
    {
        public:
            AuDevice();
            ~AuDevice();
            AuApp* launchApp(std::string appName);
            std::vector<AuApp *> getApps();
            bool click(const int x, const int y);
            bool click(const int x, const int y, const unsigned int intv);
            bool drag(const int sx, const int sy, const int ex, const int ey, const int steps, const int durationMs);
            int touchDown(const int x, const int y);
            bool touchMove(const int x, const int y, const int seq);
            bool touchUp(const int x, const int y, const int seq);
            bool pressKeyCode(std::string keyCode, KeyGenerateType type);
            bool takeScreenshot(std::string path, float scale, int quality);
            long long getSystemTime(TimeType type);
            std::string getSystemLocale();

        private:
            AccessibleWatcher *mWatcher;
    };

}  // namespace aurum

#endif /* _AU_DEVICE_H_ */
