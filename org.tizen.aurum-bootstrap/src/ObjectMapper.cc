#include "ObjectMapper.h"
#include <memory>
#include "bootstrap.h"
#include <algorithm>
#include <sstream>

ObjectMapper::ObjectMapper() : mObjectMap{}, mObjCounter{0} {}

ObjectMapper::~ObjectMapper() {}

ObjectMapper *ObjectMapper::getInstance()
{
    static ObjectMapper *mInstance = new ObjectMapper();
    return mInstance;
}

std::string ObjectMapper::addElement(std::shared_ptr<UiObject> object)
{
    std::string value = object->getId();
    mObjectMap[value] = object;
    LOGI("addElement %p, id %s", object.get(), value.c_str());
    return value;
}

std::shared_ptr<UiObject> ObjectMapper::getElement(std::string elementId)
{
    LOGI("getElement for elementId(%s)", elementId.c_str());
    if (mObjectMap.count(elementId)) {
        std::shared_ptr<UiObject> obj = mObjectMap[elementId];
        obj->refresh();
        LOGI("succeeded");
        return obj;
    }
    LOGI("Id(%s) is not exist in mObjectMap", elementId.c_str());
    return nullptr;
}

bool ObjectMapper::removeElement(const std::string elementId)
{
    LOGI("removeElement for Id(%s)", elementId.c_str());
    std::shared_ptr<UiObject> obj = getElement(elementId);
    if (obj) {
        if (mObjectMap.erase(elementId))
            return true;
    }
    return false;
}

void ObjectMapper::cleanUp()
{
    LOGI("clean up object map");
    for(auto iter = mObjectMap.begin(); iter != mObjectMap.end(); ) {
        if (!iter->second->isValid()) {
            iter = mObjectMap.erase(iter);
        } else {
            ++iter;
        }
    }
}
