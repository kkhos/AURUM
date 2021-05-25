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
    std::vector<std::shared_ptr<AccessibleNode>> ret = findObjects(device, selector, root, true);
    if (ret.size() > 0)
        return std::move(ret[0]);
    else
        return nullptr;
}

std::vector<std::shared_ptr<AccessibleNode>> Comparer::findObjects(const std::shared_ptr<UiDevice> device,
                                                    const std::shared_ptr<UiSelector> selector,
                                                    const std::shared_ptr<AccessibleNode> root, bool earlyReturn)
{
    Comparer comparer(device, selector, earlyReturn);

    LOG_SCOPE_F(INFO, "findObjects selector(%s) from (type:%s style:%s, role:%s, text:%s) earlyReturn:%d", selector->description().c_str(), root->getType().c_str(),  root->getStyle().c_str(),  root->getRole().c_str(),  root->getText().c_str(), earlyReturn);

    if (selector->mParent) {
        auto ret = Comparer::findObjects(device, selector->mParent, root);
        std::vector<std::shared_ptr<AccessibleNode>> merged{};

        for (const auto &node : ret) {
            auto tmp = comparer.findObjects(node);
            std::move(std::begin(tmp), std::end(tmp), std::back_inserter(merged));
        }
        return merged;
    }

    return comparer.findObjects(root);
}

std::vector<std::shared_ptr<AccessibleNode>> Comparer::findObjects(const std::shared_ptr<AccessibleNode> root)
{
    std::list<std::shared_ptr<PartialMatch>> partialList{};
    std::vector<std::shared_ptr<AccessibleNode>> ret = findObjects(root, 0, 0, partialList);
    LOG_F(INFO, "%d object(s) found", ret.size());
    return ret;
}

std::vector<std::shared_ptr<AccessibleNode>> Comparer::findObjects(
    const std::shared_ptr<AccessibleNode> root, const int &index, const int &depth,
    std::list<std::shared_ptr<PartialMatch>> &partialMatches)
{
    std::vector<std::shared_ptr<AccessibleNode>> ret;
    //LOG_SCOPE_F(INFO, "findObjects idx:%d, depth:%d, partialMatches.size:%d", index, depth, partialMatches.size());

    root->refresh();
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
        LOG_F(INFO, "Abort searching! No need to search children(maxDepth limit overflow, %d < %d < %d)", mSelector->mMinDepth?*(mSelector->mMinDepth):-1, depth, mSelector->mMaxDepth?*(mSelector->mMaxDepth):9999999);
    }

    if (currentMatch && currentMatch->finalizeMatch()){
        LOG_F(INFO, "Found matched = %s with criteria %s", root->description().c_str(), currentMatch->debugPrint().c_str());
        ret.push_back(root);
    }

    return ret;
}
