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

#include "../inc/Aurum.h";
#include <app_control.h>

using namespace aurum;
using namespace std;

AuDevice::AuDevice()
{
    dlog_print(DLOG_INFO, LOG_TAG, "AuDevice Constructor");
    this->mWatcher = AccessibleWatcher::getInstance();
}
AuDevice::~AuDevice()
{
    dlog_print(DLOG_INFO, LOG_TAG, "AuDevice Destructor");
}

//FIXME: Applaunched is not called...
void appLaunched(app_control_h request, app_control_h reply, app_control_result_e result, void *user_data)
{
    switch (result) {
    case APP_CONTROL_RESULT_SUCCEEDED:
        dlog_print(DLOG_INFO, LOG_TAG, "APP_CONTROL_RESULT_SUCCEEDED");
        break;
    case APP_CONTROL_RESULT_FAILED:
        dlog_print(DLOG_INFO, LOG_TAG, "APP_CONTROL_RESULT_FAILED");
        break;
    case APP_CONTROL_RESULT_CANCELED:
        dlog_print(DLOG_INFO, LOG_TAG, "APP_CONTROL_RESULT_CANCELED");
        break;
    default:
        dlog_print(DLOG_INFO, LOG_TAG, "unhandled value %d", result);
        break;
    }
}

AuApp* AuDevice::launchApp(std::string appName)
{
    dlog_print(DLOG_INFO, LOG_TAG, "launchApp Start : %s", appName.c_str());
    app_control_h appControl;
    int           ret = -1;

    ret = app_control_create(&appControl);
    if (ret) {
        dlog_print(DLOG_ERROR, LOG_TAG, "Launch Failed(1/3)");
        return NULL;
    }

    ret = app_control_set_app_id(appControl, appName.c_str());
    if (ret) {
       dlog_print(DLOG_ERROR, LOG_TAG, "Launch Failed(2/3)");
       app_control_destroy(appControl);
       return NULL;
    }

    ret = app_control_send_launch_request(appControl, appLaunched, NULL);

    if (ret) {
       dlog_print(DLOG_ERROR, LOG_TAG, "Launch Failed(3/3)");
       app_control_destroy(appControl);
       return NULL;
    }

    app_control_destroy(appControl);

    std::this_thread::sleep_for(std::chrono::milliseconds{1000});

    //Create App instance
    auto apps = this->mWatcher->getActiveApplications();
    for (auto &app : apps){
        if (app->getPackageName().compare(appName))
        {
            AuApp *auApp = new AuApp(this->mWatcher, app.get());
            auApp->setName(appName);
            dlog_print(DLOG_INFO, LOG_TAG, "launchApp Success return AuApp(%p)", auApp);
            return auApp;
        }
    }

    dlog_print(DLOG_INFO, LOG_TAG, "launchApp fail");
    return NULL;
}

std::vector<AuApp *> AuDevice::getApps()
{
    std::vector<AuApp *> ret{};
    return ret;
}

bool AuDevice::click(const int x, const int y)
{
    return false;
}

bool AuDevice::click(const int x, const int y, const unsigned int intv)
{
    return false;
}

bool AuDevice::drag(const int sx, const int sy, const int ex, const int ey, const int steps, const int durationMs)
{
    return false;
}

int AuDevice::touchDown(const int x, const int y)
{
    return 0;
}

bool AuDevice::touchMove(const int x, const int y, const int seq)
{
    return false;
}

bool AuDevice::touchUp(const int x, const int y, const int seq)
{
    return false;
}

bool AuDevice::pressKeyCode(std::string keyCode, KeyGenerateType type)
{
    return false;
}

bool AuDevice::takeScreenshot(std::string path, float scale, int quality)
{
    return false;
}

long long AuDevice::getSystemTime(TimeType type)
{
    return 0;
}

std::string AuDevice::getSystemLocale()
{
    return NULL;
}