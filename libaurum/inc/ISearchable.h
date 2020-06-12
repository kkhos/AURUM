#ifndef ISEARCHABLE_H
#define ISEARCHABLE_H
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
    virtual std::unique_ptr<UiObject> findObject(
        const std::shared_ptr<UiSelector> selector) const = 0;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual std::vector<std::unique_ptr<UiObject>> findObjects(
        const std::shared_ptr<UiSelector> selector) const = 0;
};

#endif