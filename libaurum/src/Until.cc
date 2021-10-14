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

std::function<bool(const ISearchable *)> Until::hasObject(
    const std::shared_ptr<UiSelector> selector)
{
    return [=](const ISearchable *searchable) -> bool {
        LOGI("hasObject search:%p", searchable);
        std::shared_ptr<UiObject> obj = searchable->findObject(selector);
        return obj.get() != nullptr;
    };
}

std::function<std::shared_ptr<UiObject>(const ISearchable *)> Until::findObject(
    const std::shared_ptr<UiSelector> selector)
{
    return [=](const ISearchable *searchable) -> std::shared_ptr<UiObject> {
        LOGI("findObject search:%p", searchable);
        std::shared_ptr<UiObject> obj = searchable->findObject(selector);
        return obj;
    };
}

std::function<bool(const UiObject *)> Until::checkable(const bool isCheckable)
{
    return [=](const UiObject *object) -> bool {
        LOGI("checkable checkable:%d for obj %p", isCheckable, object);
        return object->isClickable() == isCheckable;
    };
}
