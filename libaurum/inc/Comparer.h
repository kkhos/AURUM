#ifndef COMPARER_H
#define COMPARER_H
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
    Comparer(const UiDevice *device, const std::shared_ptr<UiSelector> selector,
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
    static std::unique_ptr<AccessibleNode>             findObject(const UiDevice *      device,
                                                    const std::shared_ptr<UiSelector> selector,
                                                    const AccessibleNode *root);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    static std::vector<std::unique_ptr<AccessibleNode>> findObjects(
        const UiDevice *device, const std::shared_ptr<UiSelector> selector,
        const AccessibleNode *root);

private:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::vector<std::unique_ptr<AccessibleNode>> findObjects(const AccessibleNode *root);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::vector<std::unique_ptr<AccessibleNode>> findObjects(
        const AccessibleNode *root, const int &index, const int &depth,
        std::list<std::shared_ptr<PartialMatch>> &partialMatches);

private:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::unique_ptr<PartialMatch> accept(const AccessibleNode *node,
                                         const std::shared_ptr<UiSelector> selector,
                                         const int &index, const int &depth,
                                         const int &relDepth);

private:
    /**
     * @brief TBD
     */
    const UiDevice *mDevice;

    /**
     * @brief TBD
     */
    const std::shared_ptr<UiSelector> mSelector;

    /**
     * @brief TBD
     */
    bool              mEarlyReturn;
};

#endif
