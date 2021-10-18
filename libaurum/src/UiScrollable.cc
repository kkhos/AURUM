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

using namespace Aurum;

UiScrollable::UiScrollable(std::shared_ptr<UiSelector> selector)
: mSelector{selector}, mMaxSearchSwipe{100}, mScrollStep{50}
{
}

UiScrollable::UiScrollable()
: mSelector{nullptr}, mMaxSearchSwipe{1}, mScrollStep{1}
{
    // todo : find the top most scrollable object
}

bool UiScrollable::scrollToObject(UiObject *obj)
{
    auto sel = obj->getSelector();

    if (exists(obj)) return true;

    for (int i = 0 ; i < mMaxSearchSwipe ; ++i) {
        bool scrolled = scrollForward();
        if (exists(obj)) return true;
        if (!scrolled) return false;
    }
}


bool UiScrollable::exists(UiObject *obj)
{
    return (obj->isShowing() && obj->isVisible());
}

bool UiScrollable::scrollForward()
{
    return scrollForward(mScrollStep);
}

bool UiScrollable::scrollForward(int steps)
{
    //TODO Check Scree Size here
    return true;

}

bool UiScrollable::scrollBackward()
{
return true;
}
bool UiScrollable::scrollBackward(int steps)
{
return true;
}

bool UiScrollable::scrollToBegin()
{
return true;
}