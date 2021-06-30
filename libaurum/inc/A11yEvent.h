#pragma once
#include "bitmask.h"
#include <string>

enum class A11yEvent {
     EVENT_NONE = 0x00000,
     EVENT_WINDOW_CREATE = 0x00001,
     EVENT_WINDOW_CLOSE = 0x00002,
     EVENT_WINDOW_MINIMIZE = 0x00004,
     EVENT_WINDOW_MAXIMIZE = 0x00008,
     EVENT_WINDOW_RESTORE = 0x00010,
     EVENT_WINDOW_ACTIVATE = 0x00020,
     EVENT_WINDOW_DEACTIVATE = 0x00040,
     EVENT_WINDOW_RAISE = 0x00080,
     EVENT_WINDOW_LOWER = 0x00100,
     EVENT_WINDOW_MOVE = 0x00200,
     EVENT_WINDOW_RESIZE = 0x00400,

     EVENT_TEXT_CHANGED_INSERT = 0x00800,
     EVENT_TEXT_CHANGED_DELETE = 0x01000,

     EVENT_STATE_CHANGED_VISIBLE = 0x02000,
     EVENT_STATE_CHANGED_FOCUSED = 0x04000,
     EVENT_STATE_CHANGED_CHECKED = 0x08000,
     EVENT_STATE_CHANGED_PRESSED = 0x10000,
     EVENT_STATE_CHANGED_SELECTED = 0x20000,
};

enableEnumClassBitfield(A11yEvent);

class A11yEventInfo {
public:
	A11yEventInfo();
	A11yEventInfo(A11yEvent event, std::string name = "", std::string pkg = "");
	A11yEventInfo(std::string event, std::string name = "", std::string pkg = "");
	~A11yEventInfo();
public:
	A11yEvent getEvent();
    A11yEvent getEvent(std::string event);
	std::string getName();
	std::string getPkg();

protected:
	const A11yEvent mEvent;
	const std::string mName;
	const std::string mPkg;
};

