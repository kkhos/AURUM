#include "ObjectMapper.h"
#include <memory>
#include <loguru.hpp>

ObjectMapper::ObjectMapper() : mObjectMap{}, mObjectMapReverse{}, mObjCounter{0} {}

ObjectMapper::~ObjectMapper() {}

ObjectMapper *ObjectMapper::getInstance()
{
    static ObjectMapper *mInstance = new ObjectMapper();
    return mInstance;
}

std::string ObjectMapper::addElement(std::shared_ptr<UiObject> object)
{
    ++mObjCounter;
    std::string key = std::to_string(mObjCounter);
    mObjectMap[key] = object;
    mObjectMapReverse[object] = key;
    LOG_SCOPE_F(INFO, "addElement %p as key %s", object.get(), key.c_str());
    return key;
}

std::shared_ptr<UiObject> ObjectMapper::getElement(std::string key)
{
    LOG_SCOPE_F(INFO, "getElement for key(%s)", key.c_str());
    unsigned long long keyCnt = (unsigned long long)std::stoll(key); // this key is a result of calling std:to_string(mObjCounter)
    if (keyCnt <= 0 || keyCnt > mObjCounter) return nullptr;
    if (mObjectMap.count(key)) {
        std::shared_ptr<UiObject> obj = mObjectMap[key];
        obj->refresh();
        LOG_F(INFO, "succeeded");
        return obj;
    }
    LOG_F(INFO, "failed(object not found)");
    return nullptr;
}

std::string ObjectMapper::getElement(std::shared_ptr<UiObject> object)
{
    LOG_SCOPE_F(INFO, "getElement for object(%p)", object.get());
    if (object && mObjectMapReverse.count(object)) {
        LOG_F(INFO, "succeeded");
        return mObjectMapReverse[object];
    }
    LOG_F(INFO, "failed(object not found)");
    return std::string{""};
}

bool ObjectMapper::removeElement(const std::string key)
{
    std::shared_ptr<UiObject> obj = getElement(key);
    if (obj) {
        if (mObjectMap.erase(key) && mObjectMapReverse.erase(obj))
            return true;
    }
    return false;
}

bool ObjectMapper::removeElement(std::shared_ptr<UiObject> object)
{
    std::string key = getElement(object);
    if (key.empty()) return false;
    return removeElement(key);
}