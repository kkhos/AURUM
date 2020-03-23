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
    UiSelector *text(const std::string &text);
    UiSelector *pkg(const std::string &text);
    UiSelector *res(const std::string &text);
    UiSelector *desc(const std::string &text);
    UiSelector *type(const std::string &text);

    UiSelector *depth(int depth);

    UiSelector *hasChild(std::shared_ptr<UiSelector> child);

public:
    std::string mText;
    std::string mPkg;
    std::string mRes;
    std::string mDesc;
    std::string mType;
    int         mDepth;

    std::vector<std::shared_ptr<UiSelector>> mChild;
};

#endif