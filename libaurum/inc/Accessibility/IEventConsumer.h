#pragma once

enum class EventType {
    none = 0,
    Object = 1,
};

enum class ObjectEventType {
    none = 0,
    ObjectStateDefunct = 1,
};

class IEventConsumer
{
public:
    virtual ~IEventConsumer() {};
    virtual void notify(int type, int type2, void *src) = 0;
};
