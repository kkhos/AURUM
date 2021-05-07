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

using namespace aurum;
using namespace std;

AuApp::AuApp(AccessibleWatcher *watcher, AccessibleApplication *app)
{
    mWatcher = watcher;
    mApp = app;
}
AuApp::~AuApp()
{
}

void AuApp::setName(std::string appName)
{
    this->mName = appName;
}

std::string AuApp::getName()
{
    return this->mName;
}

AuWindow* AuApp::getFocusedWindow()
{
    auto activeWindows = this->mApp->getActiveWindows();
    for (auto &window : activeWindows){
        AuWindow *auWin = new AuWindow(this->mWatcher, window->getAccessibleNode().get());
        auWin->setName(window->getAccessibleNode()->getText());
        dlog_print(DLOG_INFO, LOG_TAG, " Get Focused Window %p %s", auWin, window->getAccessibleNode()->getText().c_str());
        return auWin;
        
    }

    return nullptr;
}

std::vector<AuWindow *> AuApp::getWindows()
{
    std::vector<AuWindow *> ret{};
    return ret;
}
