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
#include <unistd.h>
#include <chrono>
#include <thread>

using namespace Aurum;

Waiter::Waiter() : Waiter(nullptr) {}

Waiter::~Waiter() {}

Waiter::Waiter(const ISearchable *searchableObject, const UiObject *uiObject, const int timeout)
    : mSearchableObject{searchableObject},
      mUiObject{uiObject},
      WAIT_INTERVAL_MS{100},
      WAIT_TIMEOUT_MS{timeout}
{
}

template bool Waiter::waitFor(
    const std::function<bool(const ISearchable *)> condition) const;

template std::shared_ptr<UiObject> Waiter::waitFor(
    const std::function<std::shared_ptr<UiObject>(const ISearchable *)>
        condition) const;

template bool Waiter::waitFor(
    const std::function<bool(const UiObject *)> condition) const;

template std::vector<std::shared_ptr<UiObject>> Waiter::waitFor(
    const std::function<std::vector<std::shared_ptr<UiObject>> (const ISearchable *)>
        condition) const;

template <typename R>
R Waiter::waitFor(const std::function<R(const ISearchable *)> condition) const
{
    LOGI("waitFor ISearchable");
    std::chrono::system_clock::time_point start =
        std::chrono::system_clock::now();
    R result = condition(mSearchableObject);
    while (!result) {
        if ((std::chrono::system_clock::now() - start) >
            std::chrono::milliseconds{WAIT_TIMEOUT_MS})
            break;
        std::this_thread::sleep_for(
            std::chrono::milliseconds{WAIT_INTERVAL_MS});
        result = condition(mSearchableObject);
    }
    return result;
}

template <typename R>
R Waiter::waitFor(const std::function<R(const UiObject *)> condition) const
{
    LOGI("waitFor UiObject");
    if (mUiObject) {
        std::chrono::system_clock::time_point start =
            std::chrono::system_clock::now();
        R result = condition(mUiObject);
        while (!result) {
            if ((std::chrono::system_clock::now() - start) >
                std::chrono::milliseconds{WAIT_TIMEOUT_MS})
                break;
            std::this_thread::sleep_for(
                std::chrono::milliseconds{WAIT_INTERVAL_MS});
            result = condition(mUiObject);
        }
        return result;
    }
    return R();
}

template <typename R>
std::vector<R> Waiter::waitFor(const std::function<std::vector<R>(const ISearchable *)> condition) const
{
    LOGI("waitFor ISearchable");
    std::chrono::system_clock::time_point start =
        std::chrono::system_clock::now();
    std::vector<R> result = condition(mSearchableObject);
    while (result.empty()) {
        if ((std::chrono::system_clock::now() - start) >
            std::chrono::milliseconds{WAIT_TIMEOUT_MS})
            break;
        std::this_thread::sleep_for(
            std::chrono::milliseconds{WAIT_INTERVAL_MS});
        result = condition(mSearchableObject);
    }
    return result;
}
