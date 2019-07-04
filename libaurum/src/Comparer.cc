#include "Comparer.h"

#include "loguru.hpp"

Comparer::Comparer(const UiDevice *device, const UiSelector *selector,
                   const bool &earlyReturn)
    : mDevice(device), mSelector(selector), mEarlyReturn(earlyReturn)
{
}

Comparer::~Comparer() {}

AccessibleNode *Comparer::findObject(const UiDevice *      device,
                                     const UiSelector *    selector,
                                     const AccessibleNode *root)
{
    Comparer                      comparer(device, selector, true);
    std::vector<AccessibleNode *> ret = comparer.findObjects(root);
    if (ret.size() > 0)
        return ret[0];
    else
        return nullptr;
}

std::vector<AccessibleNode *> Comparer::findObjects(const UiDevice *  device,
                                                    const UiSelector *selector,
                                                    const AccessibleNode *root)
{
    Comparer                      comparer(device, selector, false);
    std::vector<AccessibleNode *> ret = comparer.findObjects(root);
    return std::move(ret);
}

std::vector<AccessibleNode *> Comparer::findObjects(const AccessibleNode *root)
{
    std::list<std::shared_ptr<PartialMatch>> partialList{};
    std::vector<AccessibleNode *> ret = findObjects(root, 0, 0, partialList);
    return std::move(ret);
}

std::vector<AccessibleNode *> Comparer::findObjects(
    const AccessibleNode *root, const int &index, const int &depth,
    std::list<std::shared_ptr<PartialMatch>> &partialMatches)
{
    std::vector<AccessibleNode *> ret;
    root->refresh();

    // LOG_F(INFO, "%p %s / i:%d d:%d", root, root->getText().c_str(), index,
    // depth);

    for (auto match : partialMatches)
        match->update(root, index, depth, partialMatches);

    std::shared_ptr<PartialMatch> currentMatch =
        PartialMatch::accept(root, mSelector, index, depth);
    if (currentMatch) partialMatches.push_front(currentMatch);

    int childCnt = root->getChildCount();
    for (int i = 0; i < childCnt; i++) {
        AccessibleNode *              childNode = root->getChildAt(i);
        std::vector<AccessibleNode *> childret =
            findObjects(childNode, i, depth + 1, partialMatches);
        ret.insert(ret.end(), childret.begin(), childret.end());

        if (!ret.empty() && mEarlyReturn) return ret;
    }

    if (currentMatch && currentMatch->finalizeMatch())
        ret.push_back(const_cast<AccessibleNode *>(root));

    return ret;
}