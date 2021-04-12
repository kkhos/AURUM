#pragma once
#include "bitmask.h"

enum class A11yEvent {
     EVENT_WINDOW_CREATE = (0x01<<0),
     EVENT_WINDOW_CLOSE = (0x01<<1),
     EVENT_WINDOW_MINIMIZE = (0x01<<2),
     EVENT_WINDOW_MAXIMIZE = (0x01<<3),
     EVENT_WINDOW_RESTORE = (0x01<<4),
     EVENT_WINDOW_ACTIVATE = (0x01<<5),
     EVENT_WINDOW_DEACTIVATE = (0x01<<6),
     EVENT_WINDOW_RAISE = (0x01<<7),
     EVENT_WINDOW_LOWER = (0x01<<8),
     EVENT_WINDOW_MOVE = (0x01<<9),
     EVENT_WINDOW_RESIZE = (0x01<<10),

     EVENT_TEXT_CHANGED_INSERTED = (0x01<<11),
     EVENT_TEXT_CHANGED_REMOVED = (0x01<<12),

     EVENT_STATE_CHANGED_VISIBLE = (0x01<<13),
     EVENT_STATE_CHANGED_FOCUSED = (0x01<<14),
     EVENT_STATE_CHANGED_CHECKED = (0x01<<15),
     EVENT_STATE_CHANGED_PRESSED = (0x01<<16),
     EVENT_STATE_CHANGED_SELECTED = (0x01<<17),
};

template<>
struct enable_bitmask_operators<A11yEvent>{
    static const bool enable=true;
};

