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
    bool isCandidate();
    void updateTS(int ts);
    float getActivity();
    void merge(Cluster *c);

    RingBuffer<Event*> *events;

    int posX;
    int posY;
    int lastPosX;
    int lastPosY;

    int lastPolarity;
    int lifeTime;

    float activity;
    unsigned int firstEventTS;  //first event assigned to this cluster
    unsigned int lastOverallEventTS;    // latest event timestamp from camera, used for liftime and activity comparison
    float velocity;
    float contour;
    bool candidate;

private:
    void update();
    void calcCentralMoment();
    void calcCountour();
};

#endif // CLUSTER_H
