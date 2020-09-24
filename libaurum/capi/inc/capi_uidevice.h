#pragma once


typedef enum _KeyRequestType {
    STROKE,
    LONG_STROKE,
    PRESS,
    RELEASE,
} KeyRequestType;

typedef enum _TimeRequestType {
    WALLCLOCK,
    MONOTONIC,
} TimeRequestType;

/**
 * @brief TBD
 * @since_tizen 5.5
 */
int aurum_uidevice_click(const int x, const int y);

/**
 * @brief TBD
 * @since_tizen 5.5
 */
int aurum_uidevice_interval_click(const int x, const int y, const unsigned int intv);

/**
 * @brief TBD
 * @since_tizen 5.5
 */
int aurum_uidevice_drag(const int sx, const int sy, const int ex, const int ey,
            const int steps, const int durationMs);

/**
 * @brief TBD
 * @since_tizen 5.5
 */
int aurum_uidevice_touchDown(const int x, const int y);

/**
 * @brief TBD
 * @since_tizen 5.5
 */
int aurum_uidevice_touchMove(const int x, const int y, const int seq);

/**
 * @brief TBD
 * @since_tizen 5.5
 */
int aurum_uidevice_touchUp(const int x, const int y, const int seq);

/**
 * @brief TBD
 * @since_tizen 5.5
 */
int aurum_uidevice_wheelUp(int amount, const int durationMs);

/**
 * @brief TBD
 * @since_tizen 5.5
 */
int aurum_uidevice_wheelDown(int amount, const int durationMs);

/**
 * @brief TBD
 * @since_tizen 5.5
 */
int aurum_uidevice_pressBack(KeyRequestType type);

/**
 * @brief TBD
 * @since_tizen 5.5
 */
int aurum_uidevice_pressHome(KeyRequestType type);

/**
 * @brief TBD
 * @since_tizen 5.5
 */
int aurum_uidevice_pressMenu(KeyRequestType type);

/**
 * @brief TBD
 * @since_tizen 5.5
 */
int aurum_uidevice_pressVolUp(KeyRequestType type);

/**
 * @brief TBD
 * @since_tizen 5.5
 */
int aurum_uidevice_pressVolDown(KeyRequestType type);

/**
 * @brief TBD
 * @since_tizen 5.5
 */
int aurum_uidevice_pressPower(KeyRequestType type);

/**
 * @brief TBD
 * @since_tizen 5.5
 */
int aurum_uidevice_pressKeyCode(char* keycode, KeyRequestType type);

/**
 * @brief TBD
 * @since_tizen 5.5
 */
int aurum_uidevice_takeScreenshot(char* path, float scale, int quality);

/**
 * @brief TBD
 * @since_tizen 5.5
 */
long long aurum_uidevice_getSystemTime(TimeRequestType type);

/**
 * @brief TBD
 * @since_tizen 5.5
 */
int aurum_uidevice_hasObject(const std::shared_ptr<UiSelector> selector);

/**
 * @brief TBD
 * @since_tizen 5.5
 */
std::shared_ptr<UiObject> aurum_uidevice_findObject(
    const std::shared_ptr<UiSelector> selector) const;

/**
 * @brief TBD
 * @since_tizen 5.5
 */
std::vector<std::shared_ptr<UiObject>> aurum_uidevice_findObjects(
    const std::shared_ptr<UiSelector> selector) const;

/**
 * @brief TBD
 * @since_tizen 5.5
 */
/*
bool aurum_uidevice_waitFor(
    const std::function<bool(const ISearchable *)> condition) const;
*/
/**
 * @brief TBD
 * @since_tizen 5.5
 */
std::shared_ptr<UiObject> aurum_uidevice_waitFor(
    const std::function<std::shared_ptr<UiObject>(const ISearchable *)>
        condition) const;
/**
 * @brief TBD
 * @since_tizen 5.5
 */
std::shared_ptr<UiDevice> aurum_uidevice_getInstance(IDevice *deviceImpl = nullptr);

/**
 * @brief TBD
 * @since_tizen 5.5
 */
std::vector<std::shared_ptr<AccessibleNode>> aurum_uidevice_getWindowRoot() const;
