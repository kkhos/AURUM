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

#include "Aurum.h"
#include <utility>

using namespace Aurum;

std::shared_ptr<UiSelector> Sel::text(std::string text)
{
    std::shared_ptr<UiSelector> sel = std::make_shared<UiSelector>();
    sel->text(text);
    return sel;
}

std::shared_ptr<UiSelector> Sel::type(std::string type)
{
    std::shared_ptr<UiSelector> sel = std::make_shared<UiSelector>();
    sel->type(type);
    return sel;
}

std::shared_ptr<UiSelector> Sel::style(std::string style)
{
    std::shared_ptr<UiSelector> sel = std::make_shared<UiSelector>();
    sel->style(style);
    return sel;
}

std::shared_ptr<UiSelector> Sel::depth(int depth)
{
    std::shared_ptr<UiSelector> sel = std::make_shared<UiSelector>();
    sel->depth(depth);
    return sel;
}