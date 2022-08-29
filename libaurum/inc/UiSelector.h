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

namespace Aurum {

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
     * @since_tizen 6.5
     */
    UiSelector();

    /**
     * @brief UiSelector constructor with selector.
     * @since_tizen 6.5
     */
    UiSelector(UiSelector &selector);

    /**
     * @brief Gets Selector information as string.
     *
     * @since_tizen 6.5
     */
    std::string description();

public:
    /**
     * @brief Sets the search criteria to match the object's Id.
     *
     * @param[in] text object Id
     *
     * @return UiSelector pointer
     *
     * @since_tizen 6.5
     */
    UiSelector *id(std::string text);

    /**
     * @brief Sets the search criteria to match the object's automationid.
     *
     * @param[in] text object automationid
     *
     * @return UiSelector pointer
     *
     * @since_tizen 6.5
     */
    UiSelector *automationid(std::string text);

    /**
     * @brief Sets the search criteria to match the object's text.
     *
     * @param[in] text object text
     *
     * @return UiSelector class instance
     *
     * @since_tizen 6.5
     */
    UiSelector *text(std::string text);

    /**
     * @brief Sets the search criteria to match the object's ocr text.
     *
     * @param[in] text object text
     *
     * @return UiSelector class instance
     *
     * @since_tizen 7.0
     */
    UiSelector *ocrText(std::string text);

    /**
     * @brief Sets the search criteria to match the object's text has given text.
     *
     * @param[in] text object text
     *
     * @return UiSelector class instance
     *
     * @since_tizen 6.5
     */
    UiSelector *textPartialMatch(std::string text);

    /**
     * @brief Sets the search criteria to match the object's package name.
     *
     * @param[in] text object package name
     *
     * @return UiSelector class instance
     *
     * @since_tizen 6.5
     */
    UiSelector *pkg(std::string text);

    /**
     * @brief Sets the search criteria to match the object's role.
     *
     * @param[in] text object role name
     *
     * @return UiSelector class instance
     *
     * @since_tizen 6.5
     */
    UiSelector *role(std::string text);

    /**
     * @brief Sets the search criteria to match the object's type.
     *
     * @param[in] text object type name
     *
     * @return UiSelector class instance
     *
     * @since_tizen 6.5
     */
    UiSelector *type(std::string text);

    /**
     * @brief Sets the search criteria to match the object's style.
     *
     * @param[in] text object style name
     *
     * @return UiSelector class instance
     *
     * @since_tizen 6.5
     */
    UiSelector *style(std::string text);

    /**
     * @brief Sets the depth of the selector.
     *
     * @param[in] depth depth
     *
     * @return UiSelector class instance
     *
     * @since_tizen 6.5
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
     * @since_tizen 6.5
     */
    UiSelector *depth(int minDepth, int maxDepth);

    /**
     * @brief Sets the min depth of the selector.
     *
     * @param[in] depth min depth
     *
     * @return UiSelector class instance
     *
     * @since_tizen 6.5
     */
    UiSelector *minDepth(int depth);

    /**
     * @brief Sets the max depth of the selector.
     *
     * @param[in] depth max depth
     *
     * @return UiSelector class instance
     *
     * @since_tizen 6.5
     */
    UiSelector *maxDepth(int depth);

    /**
     * @brief Sets the search criteria to match the object that is currently checked.
     *
     * @param[in] condition object's checked condition
     *
     * @return UiSelector class instance
     *
     * @since_tizen 6.5
     */
    UiSelector *isChecked(bool condition);

    /**
     * @brief Sets the search criteria to match the object that is checkable.
     *
     * @param[in] condition object's checkable condition
     *
     * @return UiSelector class instance
     *
     * @since_tizen 6.5
     */
    UiSelector *isCheckable(bool condition);

    /**
     * @brief Sets the search criteria to match the object that is clickable.
     *
     * @param[in] condition object's clickable condition
     *
     * @return UiSelector class instance
     *
     * @since_tizen 6.5
     */
    UiSelector *isClickable(bool condition);

    /**
     * @brief Sets the search criteria to match the object that is currently checked.
     *
     * @param[in] condition object's checked condition
     *
     * @return UiSelector class instance
     *
     * @since_tizen 6.5
     */
    UiSelector *isEnabled(bool condition);

    /**
     * @brief Sets the search criteria to match the object that is currently focused.
     *
     * @param[in] condition object's focused condition
     *
     * @return UiSelector class instance
     *
     * @since_tizen 6.5
     */
    UiSelector *isFocused(bool condition);

    /**
     * @brief Sets the search criteria to match the object that is focusable.
     *
     * @param[in] condition object's focusable condition
     *
     * @return UiSelector class instance
     *
     * @since_tizen 6.5
     */
    UiSelector *isFocusable(bool condition);

    /**
     * @brief Sets the search criteria to match the object that is scrollable.
     *
     * @param[in] condition object's scrollable condition
     *
     * @return UiSelector class instance
     *
     * @since_tizen 6.5
     */
    UiSelector *isScrollable(bool condition);

    /**
     * @brief Sets the search criteria to match the object that is currently selected.
     *
     * @param[in] condition object's selected condition
     *
     * @return UiSelector class instance
     *
     * @since_tizen 6.5
     */
    UiSelector *isSelected(bool condition);

    /**
     * @brief Sets the search criteria to match the object that is showing.
     *
     * @param[in] condition object's showing condition
     *
     * @return UiSelector class instance
     *
     * @since_tizen 6.5
     */
    UiSelector *isShowing(bool condition);

    /**
     * @brief Sets the search criteria to match the object that is currently active.
     *
     * @param[in] condition object's active condition
     *
     * @return UiSelector class instance
     *
     * @since_tizen 6.5
     */
    UiSelector *isActive(bool condition);

    /**
     * @brief Sets the search criteria to match the object that is currently visible.
     *
     * @param[in] condition object's visible condition
     *
     * @return UiSelector class instance
     *
     * @since_tizen 6.5
     */
    UiSelector *isVisible(bool condition);

    /**
     * @brief Sets the search criteria to match the object that is selectable.
     *
     * @param[in] condition object's selectable condition
     *
     * @return UiSelector class instance
     *
     * @since_tizen 6.5
     */
    UiSelector *isSelectable(bool condition);

    /**
     * @brief Sets the child selector.
     *
     * @param[in] child @UiSelector
     *
     * @return UiSelector class instance
     *
     * @since_tizen 6.5
     */
    UiSelector *hasChild(std::shared_ptr<UiSelector> child);

    /**
     * @brief Sets the parent selector.
     *
     * @param[in] parent @UiSelector
     *
     * @return UiSelector class instance
     *
     * @since_tizen 6.5
     */
    UiSelector *fromParent(std::shared_ptr<UiSelector> parent);

    /**
     * @brief Sets the search criteria to match the object's XPath.
     *
     * @param[in] xpath object XPath
     *
     * @return UiSelector class instance
     *
     * @since_tizen 7.0
     */
    UiSelector *xpath(std::string xpath);

    /**
     * @brief Sets the search criteria to match the object's geometry.
     *
     * @param[in] geometry Rect<int>
     * @param[in] isEqual bool
     *
     * @return UiSelector class instance
     *
     * @since_tizen 7.0
     */
    UiSelector *geometry(Rect<int> geometry, bool isEqual);

public:
    std::string mId;
    std::string mAutomationId;
    std::string mRole;
    std::string mText;
    std::string mPkg;
    std::string mType;
    std::string mStyle;
    std::string mTextPartialMatch;
    std::string mXPath;
    std::string mOcrText;

    bool mMatchId;
    bool mMatchAutomationId;
    bool mMatchRole;
    bool mMatchText;
    bool mMatchPkg;
    bool mMatchType;
    bool mMatchStyle;
    bool mMatchTextPartialMatch;
    bool mMatchXPath;
    bool mMatchOcrText;
    bool mMatchGeometry;

    bool mMatchChecked;
    bool mMatchCheckable;
    bool mMatchClickable;
    bool mMatchEnabled;
    bool mMatchFocused;
    bool mMatchFocusable;
    bool mMatchScrollable;
    bool mMatchSelected;
    bool mMatchShowing;
    bool mMatchActive;
    bool mMatchVisible;
    bool mMatchSelectable;

    int mMinDepth;
    int mMaxDepth;

    bool mIschecked;
    bool mIscheckable;
    bool mIsclickable;
    bool mIsenabled;
    bool mIsfocused;
    bool mIsfocusable;
    bool mIsscrollable;
    bool mIsselected;
    bool mIsshowing;
    bool mIsactive;
    bool mIsvisible;
    bool mIsselectable;

    bool mGeometryIsEqual;

    std::vector<std::shared_ptr<UiSelector>> mChild;
    std::shared_ptr<UiSelector> mParent;

    Rect<int> mGeometry;
};

}

#endif
