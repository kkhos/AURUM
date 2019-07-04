#ifndef UNTIL_H
#define UNTIL_H

#include <functional>
#include "ISearchable.h"
#include "UiSelector.h"

class Until {
private:
    Until();
    Until(const UiSelector *selector);
    Until(const Until &src);
    Until(const Until &&src);

public:
    ~Until();

public:
    static std::function<bool(const ISearchable *)> hasObject(
        const UiSelector *selector);
    static std::function<std::unique_ptr<UiObject>(const ISearchable *)>
                                                 findObject(const UiSelector *selector);
    static std::function<bool(const UiObject *)> checkable(
        const bool isCheckable);
};
#endif

// Until::hasObject(Sel::text("text").get())