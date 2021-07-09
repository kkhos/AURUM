#include "ObjectMapper.h"
#include <memory>
#include "bootstrap.h"
#include <algorithm>
#include <sstream>

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
    std::string value = object->getId();
    mObjectMapReverse[value] = key;
    LOGI("addElement %p as key %s, id %s", object.get(), key.c_str(), value.c_str());
    return key;
}

std::shared_ptr<UiObject> ObjectMapper::getElement(std::string key)
{
    LOGI("getElement for key(%s)", key.c_str());
    unsigned long long keyCnt = (unsigned long long)std::stoll(key); // this key is a result of calling std:to_string(mObjCounter)
    if (keyCnt <= 0 || keyCnt > mObjCounter) return nullptr;
    if (mObjectMap.count(key)) {
        std::shared_ptr<UiObject> obj = mObjectMap[key];
        obj->refresh();
        LOGI("succeeded");
        return obj;
    }
    LOGI("key is not exist in mObjectMap");
    return nullptr;
}

std::string ObjectMapper::getElement(std::shared_ptr<UiObject> object)
{
    LOGI("getElement for object(%p)", object.get());
    std::string value = object->getId();
    if (mObjectMapReverse.count(value)) {
        LOGI("succeeded");
        return mObjectMapReverse[value];
    }
    LOGI("object is not exist in mObjectMapReverse");
    return std::string{""};
}

bool ObjectMapper::removeElement(const std::string key)
{
    LOGI("removeElement for key(%s)", key.c_str());
    std::shared_ptr<UiObject> obj = getElement(key);
    if (obj) {
        std::string value = obj->getId();
        if (mObjectMap.erase(key) && mObjectMapReverse.erase(value))
            return true;
    }
    return false;
}

bool ObjectMapper::removeElement(std::shared_ptr<UiObject> object)
{
    LOGI("removeElement for object(%p)", object.get());
    std::string key = getElement(object);
    if (key.empty()) return false;
    return removeElement(key);
}

void ObjectMapper::cleanUp()
{
    LOGI("clean up object map");
    for(auto iter = mObjectMapReverse.begin(); iter != mObjectMapReverse.end(); ) {
	    auto obj = mObjectMap[iter->second];
        if (obj && !obj->isValid()) {
            iter = mObjectMapReverse.erase(iter);
        } else {
            ++iter;
        }
    }
    for(auto iter = mObjectMap.begin(); iter != mObjectMap.end(); ) {
        if (!iter->second->isValid()) {
            iter = mObjectMap.erase(iter);
        } else {
            ++iter;
        }
    }
}
