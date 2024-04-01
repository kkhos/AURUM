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

#include <unordered_map>

using namespace Aurum;

A11yEventInfo::A11yEventInfo() : A11yEventInfo(A11yEvent::EVENT_NONE, nullptr, nullptr) {}

A11yEventInfo::~A11yEventInfo()
{
    LOGI("A11yEventInfo destruct name: %s, pkg: %s", mName.c_str(), mPkg.c_str());
}

A11yEventInfo::A11yEventInfo(A11yEvent event, std::string name, std::string pkg)
	: mEvent(event),
	  mName(name),
	  mPkg(pkg)
{
    LOGI("A11yEventInfo construct name: %s, pkg: %s", name.c_str(), pkg.c_str());
}

A11yEventInfo::A11yEventInfo(std::string event, std::string name, std::string pkg)
	: mEvent(getEvent(event)),
	  mName(name),
	  mPkg(pkg)
{
    LOGI("A11yEventInfo construct event: %s, name: %s, pkg: %s", event.c_str(), name.c_str(), pkg.c_str());
}


static std::unordered_map<std::string,A11yEvent> const table = { {"window:create", A11yEvent::EVENT_WINDOW_CREATE},
																 {"window:destroy", A11yEvent::EVENT_WINDOW_DESTROY},
																 {"window:minimize", A11yEvent::EVENT_WINDOW_MINIMIZE},
																 {"window:maximize", A11yEvent::EVENT_WINDOW_MAXIMIZE},
 																 {"window:restore", A11yEvent::EVENT_WINDOW_RESTORE},
																 {"window:activate", A11yEvent::EVENT_WINDOW_ACTIVATE},
																 {"window:deactivate", A11yEvent::EVENT_WINDOW_DEACTIVATE},
																 {"window:raise", A11yEvent::EVENT_WINDOW_RAISE},
																 {"window:lower", A11yEvent::EVENT_WINDOW_LOWER},
																 {"window:resize", A11yEvent::EVENT_WINDOW_RESIZE},
																 {"window:move", A11yEvent::EVENT_WINDOW_MOVE},
																 {"window:post-render", A11yEvent::EVENT_WINDOW_RENDER_POST},
																 {"object:text-changed:insert", A11yEvent::EVENT_TEXT_CHANGED_INSERT},
																 {"object:text-changed:delete", A11yEvent::EVENT_TEXT_CHANGED_DELETE},
																 {"object:state-changed:VISIBLE", A11yEvent::EVENT_STATE_CHANGED_VISIBLE},
																 {"object:state-changed:focused", A11yEvent::EVENT_STATE_CHANGED_FOCUSED},
																 {"object:state-changed:checked", A11yEvent::EVENT_STATE_CHANGED_CHECKED},
																 {"object:state-changed:pressed", A11yEvent::EVENT_STATE_CHANGED_PRESSED},
																 {"object:state-changed:selected", A11yEvent::EVENT_STATE_CHANGED_SELECTED} };

A11yEvent A11yEventInfo::getEvent()
{
	return mEvent;
}

A11yEvent A11yEventInfo::getEvent(std::string event)
{
	auto it = table.find(event);
	if (it != table.end()) {
		return it->second;
	}
	return A11yEvent::EVENT_NONE;
}

std::string A11yEventInfo::getName()
{
	return mName;
}

std::string A11yEventInfo::getPkg()
{
	return mPkg;
}
