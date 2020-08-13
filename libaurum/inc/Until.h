#ifndef UNTIL_H
#define UNTIL_H

#include <functional>
#include "ISearchable.h"
#include "UiSelector.h"
/**
 * @brief Until class
 * @since_tizen 5.5
 */
class Until {
private:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    Until();

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    Until(const std::shared_ptr<UiSelector> selector);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    Until(const Until &src);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    Until(const Until &&src);

public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    ~Until();

public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    static std::function<bool(const ISearchable *)> hasObject(
        const std::shared_ptr<UiSelector> selector);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    static std::function<std::shared_ptr<UiObject>(const ISearchable *)>
                                                 findObject(const std::shared_ptr<UiSelector> selector);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    static std::function<bool(const UiObject *)> checkable(
        const bool isCheckable);
};
#endif

// Until::hasObject(Sel::text("text").get())