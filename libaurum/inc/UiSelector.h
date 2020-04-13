#ifndef UI_SELECTOR_H
#define UI_SELECTOR_H
#include "config.h"

#include <string>
#include <vector>
#include <memory>

class UiSelector {
public:
    UiSelector();
    UiSelector(UiSelector &selector);

    // UiSelector(const UiSelector &src);
    //        UiSelector &operator= (const UiSelector& src);

public:
    UiSelector *id(std::string text);
    UiSelector *text(std::string text);
    UiSelector *pkg(std::string text);

    UiSelector *type(std::string text);
    UiSelector *style(std::string text);

    UiSelector *depth(int depth);
    UiSelector *depth(int minDepth, int maxDepth);
    UiSelector *minDepth(int depth);
    UiSelector *maxDepth(int depth);

    UiSelector *isChecked(bool cond);
    UiSelector *isCheckable(bool cond);
    UiSelector *isClickable(bool cond);
    UiSelector *isEnabled(bool cond);
    UiSelector *isFocused(bool cond);
    UiSelector *isFocusable(bool cond);
    UiSelector *isScrollable(bool cond);
    UiSelector *isSelected(bool cond);
    UiSelector *isShowing(bool cond);
    UiSelector *isActive(bool cond);

    UiSelector *res(std::string text);
    UiSelector *desc(std::string text);

    UiSelector *hasChild(std::shared_ptr<UiSelector> child);

public:
    std::unique_ptr<std::string> mId;
    std::unique_ptr<std::string> mText;
    std::unique_ptr<std::string> mPkg;

    std::unique_ptr<std::string> mType;
    std::unique_ptr<std::string> mStyle;

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

    std::unique_ptr<std::string> mRes;
    std::unique_ptr<std::string> mDesc;

    std::vector<std::shared_ptr<UiSelector>> mChild;
};

#endif