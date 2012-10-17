#ifndef CLUSTER_H
#define CLUSTER_H

#include "ringbuffer.h"
#include "event.h"

class Cluster
{
public:
    Cluster();
    ~Cluster();
    void addEvent(Event *e);
    void update();
    void update(Event *e);
    void calcMoment();

    int posX;
    int posY;
    int avgTime;
    int polarity;
    int lifeTime;
    int activty;
    int lastEventTime;
    //velocity
    //contour

private:
    RingBuffer<Event*> *events;
};

#endif // CLUSTER_H
