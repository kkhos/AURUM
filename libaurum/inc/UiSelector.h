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

#ifndef _UI_SELECTOR_H_
#define _UI_SELECTOR_H_

#include "config.h"

#include <string>
#include <vector>
#include <memory>

/**
 * @class UiSelector
 *
 * @ingroup aurum
 *
 * @brief Specifies the elements in the layout hierarchy for tests to target,
 *        filtered by properties such as text value,
 *        content-description, class name, and state information.
 *        User can also target an element by its location in a layout hierarchy.
 */
class UiSelector {
public:
    /**
     * @brief UiSelector constructor.
     *
     * @since_tizen 5.5
     */
    UiSelector();

    /**
     * @brief UiSelector constructor with selector.
     * @since_tizen 5.5
     */
    UiSelector(UiSelector &selector);

    /**
     * @brief Gets Selector information as string.
     *
     * @since_tizen 5.5
     */
    std::string description();

public:
    /**
     * @brief Sets the search criteria to match the object's Id.
     *
     * @param[in] text object Id
     * @param[in] match check Id is a search criteria or not (default = true)
     *
     * @return UiSelector pointer
     *
     * @since_tizen 5.5
     */
    UiSelector *id(std::string text, bool match = true);

    /**
     * @brief Sets the search criteria to match the object's automationid.
     *
     * @param[in] text object automationid
     * @param[in] match check automationid is a search criteria or not (default = true)
     *
     * @return UiSelector pointer
     *
     * @since_tizen 5.5
     */
    UiSelector *automationid(std::string text, bool match = true);

    /**
     * @brief Sets the search criteria to match the object's text.
     *
     * @param[in] text object text
     * @param[in] match check text is a search criteria or not (default = true)
     *
     * @return UiSelector class instance
     *
     * @since_tizen 5.5
     */
    UiSelector *text(std::string text, bool match = true);

    /**
     * @brief Sets the search criteria to match the object's package name.
     *
     * @param[in] text object package name
     * @param[in] match check package name is a search criteria or not (default = true)
     *
     * @return UiSelector class instance
     *
     * @since_tizen 5.5
     */
    UiSelector *pkg(std::string text, bool match = true);

    /**
     * @brief Sets the search criteria to match the object's role.
     *
     * @param[in] text object role name
     * @param[in] match check role name is a search criteria or not (default = true)
     *
     * @return UiSelector class instance
     *
     * @since_tizen 5.5
     */
    UiSelector *role(std::string text, bool match = true);

    /**
     * @brief Sets the search criteria to match the object's type.
     *
     * @param[in] text object type name
     * @param[in] match check type name is a search criteria or not (default = true)
     *
     * @return UiSelector class instance
     *
     * @since_tizen 5.5
     */
    UiSelector *type(std::string text, bool match = true);

    /**
     * @brief Sets the search criteria to match the object's style.
     *
     * @param[in] text object style name
     * @param[in] match check style name is a search criteria or not (default = true)
     *
     * @return UiSelector class instance
     *
     * @since_tizen 5.5
     */
    UiSelector *style(std::string text, bool match = true);

    /**
     * @brief Sets the depth of the selector.
     *
     * @param[in] depth depth
     *
     * @return UiSelector class instance
     *
     * @since_tizen 5.5
     */
    UiSelector *depth(int depth);

    /**
     * @brief Sets the min and max depth of the selector.
     *
     * @param[in] minDepth min depth
     * @param[in] maxDepth max depth
     *
     * @return UiSelector class instance
     *
     * @since_tizen 5.5
     */
    UiSelector *depth(int minDepth, int maxDepth);

    /**
     * @brief Sets the min depth of the selector.
     *
     * @param[in] depth min depth
     *
     * @return UiSelector class instance
     *
     * @since_tizen 5.5
     */
    UiSelector *minDepth(int depth);

    /**
     * @brief Sets the max depth of the selector.
     *
     * @param[in] depth max depth
     *
     * @return UiSelector class instance
     *
     * @since_tizen 5.5
     */
    UiSelector *maxDepth(int depth);

    /**
     * @brief Sets the search criteria to match the object that is currently checked.
     *
     * @param[in] condition object's checked condition
     *
     * @return UiSelector class instance
     *
     * @since_tizen 5.5
     */
    UiSelector *isChecked(bool condition);

    /**
     * @brief Sets the search criteria to match the object that is checkable.
     *
     * @param[in] condition object's checkable condition
     *
     * @return UiSelector class instance
     *
     * @since_tizen 5.5
     */
    UiSelector *isCheckable(bool condition);

    /**
     * @brief Sets the search criteria to match the object that is clickable.
     *
     * @param[in] condition object's clickable condition
     *
     * @return UiSelector class instance
     *
     * @since_tizen 5.5
     */
    UiSelector *isClickable(bool condition);

    /**
     * @brief Sets the search criteria to match the object that is currently checked.
     *
     * @param[in] condition object's checked condition
     *
     * @return UiSelector class instance
     *
     * @since_tizen 5.5
     */
    UiSelector *isEnabled(bool condition);

    /**
     * @brief Sets the search criteria to match the object that is currently focused.
     *
     * @param[in] condition object's focused condition
     *
     * @return UiSelector class instance
     *
     * @since_tizen 5.5
     */
    UiSelector *isFocused(bool condition);

    /**
     * @brief Sets the search criteria to match the object that is focusable.
     *
     * @param[in] condition object's focusable condition
     *
     * @return UiSelector class instance
     *
     * @since_tizen 5.5
     */
    UiSelector *isFocusable(bool condition);

    /**
     * @brief Sets the search criteria to match the object that is scrollable.
     *
     * @param[in] condition object's scrollable condition
     *
     * @return UiSelector class instance
     *
     * @since_tizen 5.5
     */
    UiSelector *isScrollable(bool condition);

    /**
     * @brief Sets the search criteria to match the object that is currently selected.
     *
     * @param[in] condition object's selected condition
     *
     * @return UiSelector class instance
     *
     * @since_tizen 5.5
     */
    UiSelector *isSelected(bool condition);

    /**
     * @brief Sets the search criteria to match the object that is showing.
     *
     * @param[in] condition object's showing condition
     *
     * @return UiSelector class instance
     *
     * @since_tizen 5.5
     */
    UiSelector *isShowing(bool condition);

    /**
     * @brief Sets the search criteria to match the object that is currently active.
     *
     * @param[in] condition object's active condition
     *
     * @return UiSelector class instance
     *
     * @since_tizen 5.5
     */
    UiSelector *isActive(bool condition);

    /**
     * @brief Sets the search criteria to match the object that is currently visible.
     *
     * @param[in] condition object's visible condition
     *
     * @return UiSelector class instance
     *
     * @since_tizen 5.5
     */
    UiSelector *isVisible(bool condition);

    /**
     * @brief Sets the search criteria to match the object that is selectable.
     *
     * @param[in] condition object's selectable condition
     *
     * @return UiSelector class instance
     *
     * @since_tizen 5.5
     */
    UiSelector *isSelectable(bool condition);

    /**
     * @brief Sets the child selector.
     *
     * @param[in] child @UiSelector
     *
     * @return UiSelector class instance
     *
     * @since_tizen 5.5
     */
    UiSelector *hasChild(std::shared_ptr<UiSelector> child);

    /**
     * @brief Sets the parent selector.
     *
     * @param[in] parent @UiSelector
     *
     * @return UiSelector class instance
     *
     * @since_tizen 5.5
     */
    UiSelector *fromParent(std::shared_ptr<UiSelector> parent);

public:
    std::unique_ptr<std::string> mId;
    std::unique_ptr<std::string> mAutomationId;
    std::unique_ptr<std::string> mRole;
    std::unique_ptr<std::string> mText;
    std::unique_ptr<std::string> mPkg;
    std::unique_ptr<std::string> mType;
    std::unique_ptr<std::string> mStyle;
    std::unique_ptr<bool> mMatchId;
    std::unique_ptr<bool> mMatchAutomationId;
    std::unique_ptr<bool> mMatchRole;
    std::unique_ptr<bool> mMatchText;
    std::unique_ptr<bool> mMatchPkg;
    std::unique_ptr<bool> mMatchType;
    std::unique_ptr<bool> mMatchStyle;
    std::unique_ptr<int> mMinDepth;
    std::unique_ptr<int> mMaxDepth;
    std::unique_ptr<bool> mIschecked;
    std::unique_ptr<bool> mIscheckable;
    std::unique_ptr<bool> mIsclickable;
    std::unique_ptr<bool> mIsenabled;
    std::unique_ptr<bool> mIsfocused;
    std::unique_ptr<bool> mIsfocusable;
    std::unique_ptr<bool> mIsscrollable;
    std::unique_ptr<bool> mIsselected;
    std::unique_ptr<bool> mIsshowing;
    std::unique_ptr<bool> mIsactive;
    std::unique_ptr<bool> mIsvisible;
    std::unique_ptr<bool> mIsselectable;
    std::vector<std::shared_ptr<UiSelector>> mChild;
    std::shared_ptr<UiSelector> mParent;
};

#endif
