#ifndef WAITER_H
#define WAITER_H

#include "ISearchable.h"

#include <functional>

class Waiter {
private:
    Waiter();

public:
    Waiter(const ISearchable *searchableObject,
           const UiObject *   uiObject = nullptr);  // : mObject{object}
    ~Waiter();

public:
    template <typename R>
    R waitFor(const std::function<R(const ISearchable *)> condition) const;
    template <typename R>
    R waitFor(const std::function<R(const UiObject *)> object) const;

private:
    const ISearchable *mSearchableObject;
    const UiObject *   mUiObject;
    const int          WAIT_INTERVAL_MS;
    const int          WAIT_TIMEOUT_MS;
};

#endif