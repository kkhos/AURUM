#pragma once

#include "ISearchable.h"

#include <functional>
/**
 * @brief Waiter class
 * @since_tizen 5.5
 */
class Waiter {
private:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    Waiter();

public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    Waiter(const ISearchable *searchableObject,
           const UiObject *uiObject = nullptr);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    ~Waiter();

public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    template <typename R>
    R waitFor(const std::function<R(const ISearchable *)> condition) const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    template <typename R>
    R waitFor(const std::function<R(const UiObject *)> object) const;

private:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    const ISearchable *mSearchableObject;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    const UiObject *mUiObject;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    const int          WAIT_INTERVAL_MS;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    const int          WAIT_TIMEOUT_MS;
};