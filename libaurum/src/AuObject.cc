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

#include "../inc/Aurum.h";

using namespace aurum;
using namespace std;

AuObject::AuObject()
{
}
AuObject::~AuObject()
{
}

void AuObject::setName(std::string objectName)
{
    this->mName = objectName;
}

std::string AuObject::getName()
{
    return this->mName;
}

std::string AuObject::getText()
{

}

void AuObject::setText(std::string text)
{

}

std::string AuObject::getType()
{
    return nullptr;
}

AuObject* AuObject::getParent()
{
    return nullptr;
}

std::vector<AuObject *> AuObject::getChildren()
{
    std::vector<AuObject *> ret{};
    return ret;
}

AuObject* AuObject::findObject()
{
    return nullptr;
}

std::vector<AuObject *> AuObject::findObjects()
{
    std::vector<AuObject *> ret{};
    return ret;
}

Rect<int> AuObject::getGeometry()
{
    Rect<int> temp;
    return temp;
}

bool AuObject::isClickable()
{
    return false;
}

bool AuObject::isFocusable()
{
    return false;
}

bool AuObject::isFocused()
{
    return false;
}

bool AuObject::isShowing()
{
    return false;
}

bool AuObject::isVisible()
{
    return false;
}

bool AuObject::click()
{
    return false;
}

void AuObject::longClick(const unsigned int intv)
{

}