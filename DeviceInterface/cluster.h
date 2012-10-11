#ifndef CLUSTER_H
#define CLUSTER_H

#include "ringbuffer.h"
#include "event.h"

class Cluster
{
public:
    Cluster();
    ~Cluster();
    void addEvent(Event event);
    void update();

    int posX;
    int posY;
    int avgTime;
    int polarity;

private:
    RingBuffer<Event> *events;
};

#endif // CLUSTER_H
