#pragma once
#include "config.h"

#include "UiSelector.h"

#include <memory>
#include <vector>

class UiObject;
/**
 * @brief ISearchable interface
 * @since_tizen 5.5
 */
class ISearchable {
public:

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual ~ISearchable() {}

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual bool hasObject(const std::shared_ptr<UiSelector> selector) const = 0;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual std::shared_ptr<UiObject> findObject(
        const std::shared_ptr<UiSelector> selector) const = 0;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual std::vector<std::shared_ptr<UiObject>> findObjects(
        const std::shared_ptr<UiSelector> selector) const = 0;
};