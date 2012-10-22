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
    void calcCentralMoment();
    void calcCountour();
    bool isCandidate();

    int posX;
    int posY;
    int lastPosX;
    int lastPosY;

    int lastPolarity;
    int lifeTime;

    float activity;
    unsigned int firstEventTS;
    unsigned int lastEventTS;
    unsigned int numEvents;  //number of events assigned to that cluster
    float velocity;
    float contour;
    bool candidate;

private:
    RingBuffer<Event*> *events;
};

#endif // CLUSTER_H
