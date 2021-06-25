#include "ObjectMapper.h"
#include <memory>
#include <loguru.hpp>
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
    LOG_SCOPE_F(INFO, "addElement %p as key %s, id %s", object.get(), key.c_str(), value.c_str());
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
    std::string value = object->getId();
    if (mObjectMapReverse.count(value)) {
        LOG_F(INFO, "succeeded");
        return mObjectMapReverse[value];
    }
    LOG_F(INFO, "failed(object not found)");
    return std::string{""};
}

bool ObjectMapper::removeElement(const std::string key)
{
    LOG_SCOPE_F(INFO, "removeElement for key(%s)", key.c_str());
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
    LOG_SCOPE_F(INFO, "removeElement for object(%p)", object.get());
    std::string key = getElement(object);
    if (key.empty()) return false;
    return removeElement(key);
}

void ObjectMapper::cleanUp()
{
    std::stringstream ss{};

    LOG_SCOPE_F(INFO, "clean up object map");
    ss << "mObjectMapReverse: ";
    for(auto iter = mObjectMapReverse.begin(); iter != mObjectMapReverse.end(); ) {
	auto obj = mObjectMap[iter->second];
        if (obj && !obj->isValid()) {
            iter = mObjectMapReverse.erase(iter);
        } else {
            ss << "(" << iter->first << "," << iter->second << ") ";
            ++iter;
        }
    }
    ss << std::endl;
    ss << "mObjectMap: ";
    for(auto iter = mObjectMap.begin(); iter != mObjectMap.end(); ) {
        if (!iter->second->isValid()) {
            iter = mObjectMap.erase(iter);
        } else {
            ss << "(" << iter->first << "," << iter->second.get() << ") ";
            ++iter;
        }
    }
    ss << std::endl;
    LOG_F(INFO, "%s", ss.str().c_str());
}

//    std::remove_if(mObjectMapReverse.begin(), mObjectMapReverse.end(), [](auto& pair){return !pair.first->isValid();});
    //auto iter = std::find_if(mObjectMap.begin(), mObjectMap.end(), [](const std::pair<std::string, std::shared_ptr<UiObject>>& pair)->bool{
        //return !pair.second->isValid();
    //});
/*
    while ( iter != mObjectMap.end()) {
        removeElement(iter->second);
        iter = std::find_if(std::next(iter), mObjectMap.end(), [](const std::pair<std::string, std::shared_ptr<UiObject>>& pair)->bool{
            return !pair.second->isValid();
        });
    }
    */
