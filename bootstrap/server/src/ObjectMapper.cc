#include "ObjectMapper.h"

ObjectMapper::ObjectMapper() : mObjectMap{}, mObjCounter{0} {}

ObjectMapper::~ObjectMapper() {}

ObjectMapper *ObjectMapper::getInstance()
{
    static ObjectMapper *mInstance = new ObjectMapper();
    return mInstance;
}

std::string ObjectMapper::addElement(std::unique_ptr<UiObject> object)
{
    ++mObjCounter;
    std::string key = std::to_string(mObjCounter);
    mObjectMap[key] = std::move(object);
    return key;
}

UiObject *ObjectMapper::getElement(const std::string &key)
{
    unsigned long long keyCnt = (unsigned long long)std::stoi(key); // this key is a result of calling std:to_string(mObjCounter)
    if (keyCnt <= 0 || keyCnt > mObjCounter) return nullptr;
    if (mObjectMap.count(key)) {
        UiObject *obj = mObjectMap[key].get();
        const_cast<const UiObject *>(obj)->refresh();
        return obj;
    }
    return nullptr;
}
