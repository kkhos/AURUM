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

#include "Aurum.h"
#include "TizenWindow.h"

using namespace Aurum;

TizenWindow::TizenWindow(int pid, Rect<int> geometry, int windowAngle, int targetAngle, bool transformed, bool alpha, int opaque, int visibility, bool focused, bool mapped, int layer, std::string name)
    : mPid(pid), mWindowGeometry(geometry), mWindowAngle(windowAngle), mTargetAngle(targetAngle),mIsTransformed(transformed), mIsAlpha(alpha), mOpaque(opaque), mVisibility(visibility), mIsFocused(focused), mIsMapped(mapped), mLayer(layer), mName(name)
{

}

TizenWindow::~TizenWindow()
{

}

int TizenWindow::getPid()
{
    return mPid;
}

Rect<int> TizenWindow::getWindowGeometry()
{
    return mWindowGeometry;
}

int TizenWindow::getWindowAngle()
{
    return mWindowAngle;
}

int TizenWindow::getTargetAngle()
{
    return mTargetAngle;
}

bool TizenWindow::isTransformed()
{
    return mIsTransformed;
}

bool TizenWindow::isAlpha()
{
    return mIsAlpha;
}

int TizenWindow::getOpaque()
{
    return mOpaque;
}

int TizenWindow::getVisibility()
{
    return mVisibility;
}

bool TizenWindow::isFocused()
{
    return mIsFocused;
}

bool TizenWindow::isMapped()
{
    return mIsMapped;
}

std::string TizenWindow::getName()
{
    return mName;
}
