#include "Comparer.h"

#include <loguru.hpp>

Comparer::Comparer(const std::shared_ptr<UiDevice> device, const std::shared_ptr<UiSelector> selector,
                   const bool &earlyReturn)
    : mDevice(device), mSelector(selector), mEarlyReturn(earlyReturn)
{
}

Comparer::~Comparer() {}

std::shared_ptr<AccessibleNode> Comparer::findObject(const std::shared_ptr<UiDevice> device,
                                     const std::shared_ptr<UiSelector> selector,
                                     const std::shared_ptr<AccessibleNode> root)
{
    Comparer                      comparer(device, selector, true);
    std::vector<std::shared_ptr<AccessibleNode>> ret = comparer.findObjects(root);
    if (ret.size() > 0)
        return std::move(ret[0]);
    else
        return nullptr;
}

std::vector<std::shared_ptr<AccessibleNode>> Comparer::findObjects(const std::shared_ptr<UiDevice> device,
                                                    const std::shared_ptr<UiSelector> selector,
                                                    const std::shared_ptr<AccessibleNode> root)
{
    Comparer                      comparer(device, selector, false);
    std::vector<std::shared_ptr<AccessibleNode>> ret = comparer.findObjects(root);
    return ret;
}

std::vector<std::shared_ptr<AccessibleNode>> Comparer::findObjects(const std::shared_ptr<AccessibleNode> root)
{
    std::list<std::shared_ptr<PartialMatch>> partialList{};
    std::vector<std::shared_ptr<AccessibleNode>> ret = findObjects(root, 0, 0, partialList);
    return ret;
}

std::vector<std::shared_ptr<AccessibleNode>> Comparer::findObjects(
    const std::shared_ptr<AccessibleNode> root, const int &index, const int &depth,
    std::list<std::shared_ptr<PartialMatch>> &partialMatches)
{
    std::vector<std::shared_ptr<AccessibleNode>> ret;
    root->refresh();
    LOG_SCOPE_F(INFO, "findObjects %s, %s, %s / i:%d d:%d / p.matches:%d", root->getText().c_str(), root->getType().c_str(), root->getStyle().c_str(), index, depth, partialMatches.size());

    for (auto &match : partialMatches)
        match->update(root, index, depth, partialMatches);

    std::shared_ptr<PartialMatch> currentMatch =
        PartialMatch::accept(root, mSelector, index, depth);
    if (currentMatch) partialMatches.push_front(currentMatch);

    if (!(mSelector->mMaxDepth && (depth+1 > *(mSelector->mMaxDepth)))) {
        int childCnt = root->getChildCount();
        for (int i = 0; i < childCnt; i++) {
            std::shared_ptr<AccessibleNode> childNode = root->getChildAt(i);
            if (childNode == nullptr) continue;

            std::vector<std::shared_ptr<AccessibleNode>> childret =
                findObjects(childNode, i, depth + 1, partialMatches);
            std::move(std::begin(childret), std::end(childret), std::back_inserter(ret));

            if (!ret.empty() && mEarlyReturn) return ret;
        }
    } else {
        LOG_F(INFO, "no need to search children(maxDepth limit overflow, %d < %d < %d)", mSelector->mMinDepth?*(mSelector->mMinDepth):-1, depth, mSelector->mMaxDepth?*(mSelector->mMaxDepth):9999999);
    }

    if (currentMatch && currentMatch->finalizeMatch()){
        LOG_F(INFO, "child 3 %p(raw:%p)", root.get(), root->getRawHandler());
        ret.push_back(root);
    }

    return ret;
}