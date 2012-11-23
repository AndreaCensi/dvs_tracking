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

    RingBuffer<Event> *events;

    float posX;
    float posY;

    int currentState;
    int lifeTime;

    bool assigned;

    unsigned int firstEventTS;  //first event assigned to this cluster
    unsigned int lastEventTS;    // latest event timestamp from camera, used for liftime and activity comparison
    float contourX;
    float contourY;
    bool candidate;

    TransitionHistory *transitionHistory;

private:
    struct Position{
        int x;
        int y;
    };

    void update();
    void extractMoments();
    void calcMoments();
    void calcCountour();
    void updateState(int ts);

    RingBuffer<int> *eventsPerInterval;
    RingBuffer<Position> *moments;
};

#endif // CLUSTER_H
