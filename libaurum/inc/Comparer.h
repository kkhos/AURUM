#pragma once

#include "config.h"

#include "AccessibleNode.h"
#include "UiDevice.h"
#include "UiSelector.h"

#include "PartialMatch.h"

#include <list>
#include <memory>
#include <vector>

/**
 * @brief Comparer class
 * @since_tizen 5.5
 */
class Comparer {
private:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    Comparer(const std::shared_ptr<UiDevice> device, const std::shared_ptr<UiSelector> selector,
             const bool &earlyReturn);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    ~Comparer();

public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    static std::shared_ptr<AccessibleNode>          findObject(const std::shared_ptr<UiDevice> device,
                                                    const std::shared_ptr<UiSelector> selector,
                                                    const std::shared_ptr<AccessibleNode> root);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    static std::vector<std::shared_ptr<AccessibleNode>> findObjects(
        const std::shared_ptr<UiDevice> device, const std::shared_ptr<UiSelector> selector,
        const std::shared_ptr<AccessibleNode> root, bool earlyReturn = false);

private:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::vector<std::shared_ptr<AccessibleNode>> findObjects(const std::shared_ptr<AccessibleNode> root);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::vector<std::shared_ptr<AccessibleNode>> findObjects(
        const std::shared_ptr<AccessibleNode> root, const int &index, const int &depth,
        std::list<std::shared_ptr<PartialMatch>> &partialMatches);

private:
    /**
     * @brief TBD
     */
    const std::shared_ptr<UiDevice> mDevice;

    /**
     * @brief TBD
     */
    const std::shared_ptr<UiSelector> mSelector;

    /**
     * @brief TBD
     */
    bool              mEarlyReturn;
};