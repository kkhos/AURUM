#include "Aurum.h"

UiScrollable::UiScrollable(std::shared_ptr<UiSelector> selector)
: mSelector{selector}, mMaxSearchSwipe{100}, mScrollStep{50}
{
}

UiScrollable::UiScrollable()
: mSelector{nullptr}, mMaxSearchSwipe{1}, mScrollStep{1}
{
    // todo : find the top most scrollable object
}

bool UiScrollable::scrollToObject(UiObject *obj)
{
    auto sel = obj->getSelector();

    if (exists(obj)) return true;

    for (int i = 0 ; i < mMaxSearchSwipe ; ++i) {
        bool scrolled = scrollForward();
        if (exists(obj)) return true;
        if (!scrolled) return false;
    }
}


bool UiScrollable::exists(UiObject *obj)
{
    return (obj->isShowing() && obj->isVisible());
}

bool UiScrollable::scrollForward()
{
    return scrollForward(mScrollStep);
}

bool UiScrollable::scrollForward(int steps)
{

    //TODO Check Scree Size here
    return true;

}

bool UiScrollable::scrollBackward()
{
return true;
}
bool UiScrollable::scrollBackward(int steps)
{
return true;
}

bool UiScrollable::scrollToBegin()
{
return true;
}