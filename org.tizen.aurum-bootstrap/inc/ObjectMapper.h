#ifndef OBJECT_MAPPER_H
#define OBJECT_MAPPER_H

#include <map>
#include <string>

#include "UiObject.h"

class ObjectMapper {
private:
    std::map<std::string, std::unique_ptr<UiObject>> mObjectMap;
    unsigned long long                               mObjCounter;

private:
    ObjectMapper();

public:
    ~ObjectMapper();

public:
    static ObjectMapper *getInstance();
    std::string          addElement(std::unique_ptr<UiObject> object);
    UiObject *           getElement(const std::string &key);
};

#endif