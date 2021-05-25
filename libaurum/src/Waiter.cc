#include "Waiter.h"
#include <unistd.h>
#include <chrono>
#include <thread>

#include "ISearchable.h"
#include "UiObject.h"
#include <loguru.hpp>

Waiter::Waiter() : Waiter(nullptr) {}

Waiter::~Waiter() {}

Waiter::Waiter(const ISearchable *searchableObject, const UiObject *uiObject)
    : mSearchableObject{searchableObject},
      mUiObject{uiObject},
      WAIT_INTERVAL_MS{500},
      WAIT_TIMEOUT_MS{5000}
{
}

template bool Waiter::waitFor(
    const std::function<bool(const ISearchable *)> condition) const;

template std::shared_ptr<UiObject> Waiter::waitFor(
    const std::function<std::shared_ptr<UiObject>(const ISearchable *)>
        condition) const;

template bool Waiter::waitFor(
    const std::function<bool(const UiObject *)> condition) const;

template <typename R>
R Waiter::waitFor(const std::function<R(const ISearchable *)> condition) const
{
    LOG_SCOPE_F(INFO, "Waiter::waitFor ISearchable");
    // startTime = currentTime();
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
    LOG_SCOPE_F(INFO, "Waiter::waitFor UiObject");
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
