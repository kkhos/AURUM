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
#include <vector>

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
	    root->updateApplication();
	    root->updatePid();
        std::string pkg = root->getPkg();
        int pid = root->getPid();
        auto XMLDoc = AccessibleWatcher::getInstance()->getXMLDoc({pkg, pid});

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
    findObjects(ret, root, partialList);
    LOGI("%d object(s) found", (int)ret.size());
}

struct findObjectsNode
{
    std::shared_ptr<AccessibleNode> curNode;
    int curIndex;
    int curDepth;
    bool isCompleted;
    int prevPartialMatchesSize = 0;
};

void Comparer::findObjects(std::vector<std::shared_ptr<AccessibleNode>> &ret,
    const std::shared_ptr<AccessibleNode>& root,
    std::list<std::shared_ptr<PartialMatch>> &partialMatches)
{
    std::vector <findObjectsNode> mStack;
    mStack.push_back({root, 0, 1, false, -1});

    while(!mStack.empty())
    {
        auto curNode = mStack.back().curNode;
        int curIndex = mStack.back().curIndex;
        int curDepth = mStack.back().curDepth;
        bool isCompleted = mStack.back().isCompleted;
        int prevPartialMatchesSize = mStack.back().prevPartialMatchesSize;
        mStack.pop_back();

        if(isCompleted)
        {
            while(partialMatches.size() > prevPartialMatchesSize)
            partialMatches.pop_front();
            continue;
        }

        if (mSelector->mMatchShowing && mSelector->mIsshowing && !curNode->isShowing()) continue;

        int partialMatchSize = (int)partialMatches.size();
        mStack.push_back({curNode, curIndex, curDepth, true, partialMatchSize});

        for (auto &match : partialMatches)
            match->update(curNode, curIndex, curDepth, partialMatches);

        std::shared_ptr<PartialMatch> currentMatch =
            PartialMatch::accept(curNode, mSelector, curIndex, curDepth);

        if (currentMatch) partialMatches.push_front(currentMatch);

        if (!(mSelector->mMaxDepth && (curDepth+1 > mSelector->mMaxDepth))) {
            auto children = curNode->getChildren();
            for (int i = (int)children.size() - 1; i >= 0; i--) {
                auto child = children[i];
                if (child->getRawHandler() == nullptr) continue;

                mStack.push_back({child, i, curDepth + 1, false, -1});
            }
        } else {
            LOGI("Abort searching! No need to search children(maxDepth limit overflow, %d < %d < %d)", mSelector->mMinDepth? mSelector->mMinDepth: -1, curDepth, mSelector->mMaxDepth?(mSelector->mMaxDepth):9999999);
        }

        if (currentMatch && currentMatch->finalizeMatch()) {
            LOGI("Found matched = %s with criteria %s", curNode->description().c_str(), currentMatch->debugPrint().c_str());
            ret.push_back(curNode);
        }

        if (!ret.empty() && mEarlyReturn) {
            LOGI("Object found and earlyReturn");
            return;
        }
    }
}
