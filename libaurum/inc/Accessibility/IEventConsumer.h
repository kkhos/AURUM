#pragma once

enum class EventType {
    Object,
};

enum class ObjectEventType {
    ObjectStateDefunct,
};

class IEventConsumer
{
public:
    virtual ~IEventConsumer() {};
    virtual void notify(int type, int type2, void *src) = 0;
};
