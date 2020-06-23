#ifndef IDEVICE_H
#define IDEVICE_H
#include "config.h"

#include <string>

/**
 * @brief TimeRequestType enum class
 * @since_tizen 5.5
 */
enum class TimeRequestType {
    WALLCLOCK,
    MONOTONIC,
};

/**
 * @brief KeyRequestType enum class
 * @since_tizen 5.5
 */
enum class KeyRequestType {
    STROKE,
    LONG_STROKE,
    PRESS,
    RELEASE,
};

/**
 * @brief IDevice interface
 * @since_tizen 5.5
 */
class IDevice {
public:
    /**
     * @brief TBD virtual dtor
     * @since_tizen 5.5
     */
    virtual ~IDevice() {}

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual bool click(const int x, const int y) = 0;

    /**
     * @brief TBD click method
     * @since_tizen 5.5
     */
    virtual bool click(const int x, const int y, const unsigned int intv) = 0;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual bool drag(const int sx, const int sy, const int ex, const int ey,
                      const int steps, const int durationMs) = 0;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual bool touchDown(const int x, const int y) = 0;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual bool touchMove(const int x, const int y) = 0;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual bool touchUp(const int x, const int y) = 0;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */

    virtual bool wheelUp(int amount, const int durationMs) = 0;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual bool wheelDown(int amount, const int durationMs) = 0;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual bool pressBack(KeyRequestType type) = 0;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual bool pressHome(KeyRequestType type) = 0;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual bool pressMenu(KeyRequestType type) = 0;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual bool pressVolUp(KeyRequestType type) = 0;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual bool pressVolDown(KeyRequestType type) = 0;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual bool pressPower(KeyRequestType type) = 0;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual bool pressKeyCode(std::string keycode, KeyRequestType type) = 0;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual bool takeScreenshot(std::string path, float scale, int quality) = 0;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual long long getSystemTime(TimeRequestType type) = 0;
};
#endif