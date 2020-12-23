#pragma once

#include "IEventConsumer.h"
#include <memory>

class IEventSource
{
public:
    virtual ~IEventSource() {};
    virtual void attach(std::shared_ptr<IEventConsumer> source) = 0;
    virtual void detach(std::shared_ptr<IEventConsumer> source) = 0;
    virtual void notifyAll(int type, int type2, void *src) = 0;
};
