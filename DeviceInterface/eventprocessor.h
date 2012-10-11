#ifndef EVENTPROCESSOR_H
#define EVENTPROCESSOR_H

#include "event.h"
#include "ringbuffer.h"
#include "cluster.h"

class EventProcessor
{
public:
    EventProcessor();
    ~EventProcessor();
    /*static*/ void processEvent(Event event);
    float distance(Event e, Cluster c);
private:
    RingBuffer<Cluster> *clusters;
};

#endif // EVENTPROCESSOR_H
