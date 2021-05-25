#include <Until.h>

#include <UiObject.h>
#include <loguru.hpp>

std::function<bool(const ISearchable *)> Until::hasObject(
    const std::shared_ptr<UiSelector> selector)
{
    return [=](const ISearchable *searchable) -> bool {
        LOG_SCOPE_F(INFO, "Until::hasObject search:%p", searchable);
        std::shared_ptr<UiObject> obj = searchable->findObject(selector);
        return obj.get() != nullptr;
    };
}

std::function<std::shared_ptr<UiObject>(const ISearchable *)> Until::findObject(
    const std::shared_ptr<UiSelector> selector)
{
    return [=](const ISearchable *searchable) -> std::shared_ptr<UiObject> {
        LOG_SCOPE_F(INFO, "Until::findObject search:%p", searchable);
        std::shared_ptr<UiObject> obj = searchable->findObject(selector);
        return obj;
    };
}

std::function<bool(const UiObject *)> Until::checkable(const bool isCheckable)
{
    return [=](const UiObject *object) -> bool {
        LOG_SCOPE_F(INFO, "Until::checkable checkable:%d for obj %p", isCheckable, object);
        return object->isClickable() == isCheckable;
    };
}
