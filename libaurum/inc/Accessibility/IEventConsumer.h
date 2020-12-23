#pragma once

enum class EventType {
    Window,
    Object,
};

enum class WindowEventType {
    WindowActivated,
    WindowDeactivated,
    WindowCreated,
    WindowDestroyed,
};

enum class ObjectEventType {
    ObjectStateVisible,
    ObjectStateDefunct,
};

class IEventConsumer
{
public:
    virtual ~IEventConsumer() {};
    virtual void notify(int type, int type2, void *src) = 0;
};
