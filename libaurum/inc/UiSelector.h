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

#pragma once

#include "config.h"

#include <string>
#include <vector>
#include <memory>
/**
 * @brief UiSelector class
 * @since_tizen 5.5
 */
class UiSelector {
public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiSelector();

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiSelector(UiSelector &selector);

    // UiSelector(const UiSelector &src);
    //        UiSelector &operator= (const UiSelector& src);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::string description();

public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiSelector *id(std::string text, bool match = true);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiSelector *automationid(std::string text, bool match = true);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiSelector *text(std::string text, bool match = true);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiSelector *pkg(std::string text, bool match = true);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiSelector *role(std::string text, bool match = true);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiSelector *type(std::string text, bool match = true);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiSelector *style(std::string text, bool match = true);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiSelector *depth(int depth);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiSelector *depth(int minDepth, int maxDepth);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiSelector *minDepth(int depth);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiSelector *maxDepth(int depth);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiSelector *isChecked(bool cond);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiSelector *isCheckable(bool cond);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiSelector *isClickable(bool cond);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiSelector *isEnabled(bool cond);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiSelector *isFocused(bool cond);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiSelector *isFocusable(bool cond);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiSelector *isScrollable(bool cond);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiSelector *isSelected(bool cond);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiSelector *isShowing(bool cond);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiSelector *isActive(bool cond);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiSelector *isVisible(bool cond);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiSelector *isSelectable(bool cond);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiSelector *hasChild(std::shared_ptr<UiSelector> child);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiSelector *fromParent(std::shared_ptr<UiSelector> parent);

public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::unique_ptr<std::string> mId;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::unique_ptr<std::string> mAutomationId;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::unique_ptr<std::string> mRole;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::unique_ptr<std::string> mText;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::unique_ptr<std::string> mPkg;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::unique_ptr<std::string> mType;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::unique_ptr<std::string> mStyle;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::unique_ptr<bool> mMatchId;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::unique_ptr<bool> mMatchAutomationId;

        /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::unique_ptr<bool> mMatchRole;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::unique_ptr<bool> mMatchText;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::unique_ptr<bool> mMatchPkg;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::unique_ptr<bool> mMatchType;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::unique_ptr<bool> mMatchStyle;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::unique_ptr<int> mMinDepth;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::unique_ptr<int> mMaxDepth;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::unique_ptr<bool> mIschecked;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::unique_ptr<bool> mIscheckable;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::unique_ptr<bool> mIsclickable;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::unique_ptr<bool> mIsenabled;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::unique_ptr<bool> mIsfocused;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::unique_ptr<bool> mIsfocusable;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::unique_ptr<bool> mIsscrollable;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::unique_ptr<bool> mIsselected;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::unique_ptr<bool> mIsshowing;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::unique_ptr<bool> mIsactive;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::unique_ptr<bool> mIsvisible;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::unique_ptr<bool> mIsselectable;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::vector<std::shared_ptr<UiSelector>> mChild;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::shared_ptr<UiSelector> mParent;
};