#pragma once
#include "UiObject.h"
#include <memory>
/**
 * @brief UiScrollable class
 * @since_tizen 5.5
 */
class UiScrollable : public UiObject
{
public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiScrollable(std::shared_ptr<UiSelector> selector);
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiScrollable();

public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool exists(UiObject *obj);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool scrollToObject(UiObject *obj);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool scrollForward();
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool scrollForward(int steps);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool scrollBackward();
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool scrollBackward(int steps);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool scrollToBegin();

private:
    /**
     * @brief TBD
     */
    std::shared_ptr<UiSelector> mSelector;

    /**
     * @brief TBD
     */
    int mMaxSearchSwipe;

    /**
     * @brief TBD
     */
    int mScrollStep;
};