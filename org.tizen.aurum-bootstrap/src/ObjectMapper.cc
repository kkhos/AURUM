/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *               http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

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

    std::shared_ptr<UiObject> obj;
    obj = mObjectMap[elementId];
    if (obj) {
        LOGI("Object find succeeded");
        return obj;
    }
    else {
        LOGI("Object find fail");
        return nullptr;
    }
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
