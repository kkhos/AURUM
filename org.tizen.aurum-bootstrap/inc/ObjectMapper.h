#pragma once

#include <map>
#include <string>

#include "UiObject.h"

class ObjectMapper {
private:
    std::map<std::string, std::shared_ptr<UiObject>> mObjectMap;
    std::map<std::string, std::string> mObjectMapReverse;
    unsigned long long                               mObjCounter;

private:
    ObjectMapper();

public:
    ~ObjectMapper();

public:
    static ObjectMapper *getInstance();
    std::string addElement(std::shared_ptr<UiObject> object);
    std::shared_ptr<UiObject> getElement(const std::string elementId);
    bool removeElement(const std::string elementId);
    void cleanUp();
};
