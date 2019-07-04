#include <Until.h>

#include <UiObject.h>
#include <loguru.hpp>

std::function<bool(const ISearchable *)> Until::hasObject(
    const UiSelector *selector)
{
    return [=](const ISearchable *searchable) -> bool {
        LOG_SCOPE_F(INFO, "sel:%p, search:%p", selector, searchable);
        std::unique_ptr<UiObject> obj = searchable->findObject(selector);
        return obj.get() != nullptr;
    };
}

std::function<std::unique_ptr<UiObject>(const ISearchable *)> Until::findObject(
    const UiSelector *selector)
{
    return [=](const ISearchable *searchable) -> std::unique_ptr<UiObject> {
        LOG_SCOPE_F(INFO, "sel:%p, search:%p", selector, searchable);
        std::unique_ptr<UiObject> obj = searchable->findObject(selector);
        return obj;
    };
}

std::function<bool(const UiObject *)> Until::checkable(const bool isCheckable)
{
    return [=](const UiObject *object) -> bool {
        LOG_SCOPE_F(INFO, "waitfor ischeckable %d for obj %p", isCheckable,
                    object);
        return object->isClickable() == isCheckable;
    };
}