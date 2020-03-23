#include "Comparer.h"

#include "loguru.hpp"

Comparer::Comparer(const UiDevice *device, const std::shared_ptr<UiSelector> selector,
                   const bool &earlyReturn)
    : mDevice(device), mSelector(selector), mEarlyReturn(earlyReturn)
{
}

Comparer::~Comparer() {}

std::unique_ptr<AccessibleNode> Comparer::findObject(const UiDevice *      device,
                                     const std::shared_ptr<UiSelector> selector,
                                     const AccessibleNode *root)
{
    Comparer                      comparer(device, selector, true);
    std::vector<std::unique_ptr<AccessibleNode>> ret = comparer.findObjects(root);
    if (ret.size() > 0)
        return std::move(ret[0]);
    else
        return nullptr;
}

std::vector<std::unique_ptr<AccessibleNode>> Comparer::findObjects(const UiDevice *  device,
                                                    const std::shared_ptr<UiSelector> selector,
                                                    const AccessibleNode *root)
{
    Comparer                      comparer(device, selector, false);
    std::vector<std::unique_ptr<AccessibleNode>> ret = comparer.findObjects(root);
    return ret;
}

std::vector<std::unique_ptr<AccessibleNode>> Comparer::findObjects(const AccessibleNode *root)
{
    std::list<std::shared_ptr<PartialMatch>> partialList{};
    std::vector<std::unique_ptr<AccessibleNode>> ret = findObjects(root, 0, 0, partialList);
    return ret;
}

std::vector<std::unique_ptr<AccessibleNode>> Comparer::findObjects(
    const AccessibleNode *root, const int &index, const int &depth,
    std::list<std::shared_ptr<PartialMatch>> &partialMatches)
{
    std::vector<std::unique_ptr<AccessibleNode>> ret;
    root->refresh();

    for (auto match : partialMatches)
        match->update(root, index, depth, partialMatches);

    std::shared_ptr<PartialMatch> currentMatch =
        PartialMatch::accept(root, mSelector, index, depth);
    if (currentMatch) partialMatches.push_front(currentMatch);

    int childCnt = root->getChildCount();
    for (int i = 0; i < childCnt; i++) {
        std::unique_ptr<AccessibleNode> childNode = root->getChildAt(i);
        std::vector<std::unique_ptr<AccessibleNode>> childret =
            findObjects(childNode.get(), i, depth + 1, partialMatches);
        std::move(std::begin(childret), std::end(childret), std::back_inserter(ret));

        if (!ret.empty() && mEarlyReturn) return ret;
    }

    if (currentMatch && currentMatch->finalizeMatch())
        ret.push_back(AccessibleNode::get(root->getAccessible()));

    return ret;
}