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
    /*static*/ void processEvent(Event e);

private:
    float distance(Event e, Cluster c);
    float getBoltzmanWeight(Event e, Cluster c);
    void assignToCluster(Event e);

    std::vector<Cluster> clusters;
};

#endif // EVENTPROCESSOR_H
