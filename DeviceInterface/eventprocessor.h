#ifndef EVENTPROCESSOR_H
#define EVENTPROCESSOR_H

#include "event.h"
#include "ringbuffer.h"
#include "cluster.h"
#include <vector>

class EventProcessor
{
public:
    EventProcessor();
    ~EventProcessor();
    /*static*/ void processEvent(Event event);
    float distance(Event e, Cluster c);
private:
    std::vector<Cluster> clusters;
};

#endif // EVENTPROCESSOR_H
