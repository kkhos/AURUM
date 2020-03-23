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

class Comparer {
private:
    Comparer(const UiDevice *device, const std::shared_ptr<UiSelector> selector,
             const bool &earlyReturn);
    ~Comparer();

public:
    static std::unique_ptr<AccessibleNode>             findObject(const UiDevice *      device,
                                                    const std::shared_ptr<UiSelector> selector,
                                                    const AccessibleNode *root);
    static std::vector<std::unique_ptr<AccessibleNode>> findObjects(
        const UiDevice *device, const std::shared_ptr<UiSelector> selector,
        const AccessibleNode *root);

private:
    std::vector<std::unique_ptr<AccessibleNode>> findObjects(const AccessibleNode *root);
    std::vector<std::unique_ptr<AccessibleNode>> findObjects(
        const AccessibleNode *root, const int &index, const int &depth,
        std::list<std::shared_ptr<PartialMatch>> &partialMatches);

private:
    std::unique_ptr<PartialMatch> accept(const AccessibleNode *node,
                                         const std::shared_ptr<UiSelector> selector,
                                         const int &index, const int &depth,
                                         const int &relDepth);

private:
    const UiDevice *mDevice;
    const std::shared_ptr<UiSelector> mSelector;
    bool              mEarlyReturn;
};

#endif
