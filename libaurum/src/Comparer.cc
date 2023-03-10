/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *               http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#include "Aurum.h"

using namespace Aurum;

Comparer::Comparer(const std::shared_ptr<UiDevice>& device, const std::shared_ptr<UiSelector>& selector,
                   const bool &earlyReturn)
    : mDevice(device), mSelector(selector), mEarlyReturn(earlyReturn)
{
}

Comparer::~Comparer() {}

std::shared_ptr<AccessibleNode> Comparer::findObject(const std::shared_ptr<UiDevice>& device,
                                     const std::shared_ptr<UiSelector>& selector,
                                     const std::shared_ptr<AccessibleNode>& root)
{
    std::vector<std::shared_ptr<AccessibleNode>> ret;
    findObjects(ret, device, selector, root, true);
    if (ret.size() > 0)
        return std::move(ret[0]);
    else
        return nullptr;
}

void Comparer::findObjects(std::vector<std::shared_ptr<AccessibleNode>> &ret,
                                                    const std::shared_ptr<UiDevice>& device,
                                                    const std::shared_ptr<UiSelector>& selector,
                                                    const std::shared_ptr<AccessibleNode>& root, bool earlyReturn)
{
    Comparer comparer(device, selector, earlyReturn);

    LOGI("findObjects selector(%s) from (type:%s style:%s, role:%s, text:%s) earlyReturn:%d", selector->description().c_str(), root->getType().c_str(),  root->getStyle().c_str(),  root->getRole().c_str(),  root->getText().c_str(), earlyReturn);
    if (selector->mParent) {

        // TODO: Optimize findObjects() when selector has a parent
        std::vector<std::shared_ptr<AccessibleNode>> ret;
        Comparer::findObjects(ret, device, selector->mParent, root);

        for (const auto &node : ret) {
            comparer.findObjects(ret, node);
        }

        return;
    }

    if (selector->mMatchXPath) {
        std::string pkg = root->getPkg();
        auto XMLDoc = AccessibleWatcher::getInstance()->getXMLDoc(pkg);

        if (XMLDoc.get() == nullptr) return;

        XMLDoc->findObjects(ret, selector->mXPath, earlyReturn);

        return;
    }

    comparer.findObjects(ret, root);
}

void Comparer::findObjects(std::vector<std::shared_ptr<AccessibleNode>> &ret,
                                                            const std::shared_ptr<AccessibleNode>& root)
{
    std::list<std::shared_ptr<PartialMatch>> partialList{};
    findObjects(ret, root, 0, 1, partialList);
    LOGI("%d object(s) found", (int)ret.size());
}

void Comparer::findObjects(std::vector<std::shared_ptr<AccessibleNode>> &ret,
    const std::shared_ptr<AccessibleNode>& root, const int &index, const int &depth,
    std::list<std::shared_ptr<PartialMatch>> &partialMatches)
{

    if (mSelector->mMatchShowing && !root->isShowing()) return;

    for (auto &match : partialMatches)
        match->update(root, index, depth, partialMatches);

    std::shared_ptr<PartialMatch> currentMatch =
        PartialMatch::accept(root, mSelector, index, depth);
    if (currentMatch) partialMatches.push_front(currentMatch);

    if (!(mSelector->mMaxDepth && (depth+1 > mSelector->mMaxDepth))) {
        int childCnt = root->getChildCount();
        for (int i = 0; i < childCnt; i++) {
            std::shared_ptr<AccessibleNode> childNode = root->getChildAt(i);
            if (childNode->getRawHandler() == nullptr) continue;

            findObjects(ret, childNode, i, depth + 1, partialMatches);
            if (!ret.empty() && mEarlyReturn) {
                LOGI("Object found and earlyReturn");
                return;
            }
        }
    } else {
        LOGI("Abort searching! No need to search children(maxDepth limit overflow, %d < %d < %d)", mSelector->mMinDepth? mSelector->mMinDepth: -1, depth, mSelector->mMaxDepth?(mSelector->mMaxDepth):9999999);
    }

    if (currentMatch && currentMatch->finalizeMatch()){
        LOGI("Found matched = %s with criteria %s", root->description().c_str(), currentMatch->debugPrint().c_str());
        ret.push_back(root);
    }
}
