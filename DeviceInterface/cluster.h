#ifndef CLUSTER_H
#define CLUSTER_H

#include "ringbuffer.h"
#include "event.h"
#include "transitionhistory.h"
#include "temporalpattern.h"

class Cluster
{
public:
    Cluster();
    ~Cluster();
    void addEvent(Event e);
    bool isCandidate();
    void updateTS(int ts);
    float getActivity();
    void convert(); //convert to candidate to feature cluster
    //void merge(Cluster *c);

    RingBuffer<Event> *events;
    RingBuffer<int> *eventsPerInterval;
    TemporalPattern pattern;

    float posX;
    float posY;
    float lastPosX;
    float lastPosY;

    int currentState;
    int lifeTime;

    bool assigned;
    bool isStatic;

//    float activity;
    unsigned int firstEventTS;  //first event assigned to this cluster
    unsigned int lastEventTS;    // latest event timestamp from camera, used for liftime and activity comparison
    float velocity;
    float contourX;
    float contourY;
    bool candidate;
    float temporalPredictor;
    int eventCount;

    TransitionHistory *transitionHistory;

private:
    void update();
    void calcMoments();
    void calcCountour();
    void updateState(int ts);
};

#endif // CLUSTER_H
