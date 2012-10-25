#ifndef CLUSTER_H
#define CLUSTER_H

#include "ringbuffer.h"
#include "event.h"
#include "transitionhistory.h"

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

    float posX;
    float posY;
    float lastPosX;
    float lastPosY;

    int lastPolarity;
    int lifeTime;

    float activity;
    unsigned int firstEventTS;  //first event assigned to this cluster
    unsigned int lastOverallEventTS;    // latest event timestamp from camera, used for liftime and activity comparison
    float velocity;
    float contourX;
    float contourY;
    bool candidate;
    float temporalPredictor;

    TransitionHistory *transitionHistory;

private:
    void update();
    void calcMoments();
    void calcCountour();
};

#endif // CLUSTER_H
