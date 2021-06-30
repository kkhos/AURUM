#include "Aurum.h"

#include <unordered_map>

A11yEventInfo::A11yEventInfo() : A11yEventInfo(A11yEvent::EVENT_NONE, nullptr, nullptr) {}

A11yEventInfo::~A11yEventInfo() {}

A11yEventInfo::A11yEventInfo(A11yEvent event, std::string name, std::string pkg)
	: mEvent(event),
	  mName(name),
	  mPkg(pkg)
{
}

A11yEventInfo::A11yEventInfo(std::string event, std::string name, std::string pkg)
	: mEvent(getEvent(event)),
	  mName(name),
	  mPkg(pkg)
{
}


static std::unordered_map<std::string,A11yEvent> const table = { {"window:create", A11yEvent::EVENT_WINDOW_CREATE},
																 {"window:close", A11yEvent::EVENT_WINDOW_CLOSE},
																 {"window:minimize", A11yEvent::EVENT_WINDOW_MINIMIZE},
																 {"window:maxmize", A11yEvent::EVENT_WINDOW_MAXIMIZE},
 																 {"window:restore", A11yEvent::EVENT_WINDOW_RESTORE},
																 {"window:activate", A11yEvent::EVENT_WINDOW_ACTIVATE},
																 {"window:deactivate", A11yEvent::EVENT_WINDOW_DEACTIVATE},
																 {"window:raise", A11yEvent::EVENT_WINDOW_RAISE},
																 {"window:lower", A11yEvent::EVENT_WINDOW_LOWER},
																 {"window:resize", A11yEvent::EVENT_WINDOW_RESIZE},
																 {"window:move", A11yEvent::EVENT_WINDOW_MOVE},
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
