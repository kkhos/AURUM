#ifndef SEL_H
#define SEL_H
#include "config.h"

#include <memory>
#include <string>

#include "UiSelector.h"
/**
 * @brief Sel class
 * @since_tizen 5.5
 */
class Sel {
public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    static std::shared_ptr<UiSelector> text(std::string text);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    static std::shared_ptr<UiSelector> type(std::string text);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    static std::shared_ptr<UiSelector> style(std::string text);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    static std::shared_ptr<UiSelector> depth(int depth);
};

#endif