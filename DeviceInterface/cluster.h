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
    void calcCentralMoment(Event *e);
    void calcCountour();

    int posX;
    int posY;
    int lastPosX;
    int lastPosY;
    int avgTime; // avgTimeStamp? => not for final cluster

    int lastPolarity;
    int lifeTime;

    float activity;
    int firstEventTS;
    int lastEventTS;
    float velocity;
    float contour;

private:
    RingBuffer<Event*> *events;
};

#endif // CLUSTER_H
