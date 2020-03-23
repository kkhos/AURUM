#ifndef ISEARCHABLE_H
#define ISEARCHABLE_H
#include "config.h"

#include "UiSelector.h"

#include <memory>
#include <vector>

class UiObject;

class ISearchable {
public:
    virtual ~ISearchable() {}
    virtual bool hasObject(const std::shared_ptr<UiSelector> selector) const = 0;
    virtual std::unique_ptr<UiObject> findObject(
        const std::shared_ptr<UiSelector> selector) const = 0;
    virtual std::vector<std::unique_ptr<UiObject>> findObjects(
        const std::shared_ptr<UiSelector> selector) const = 0;
};

#endif