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

// private:
    struct Moment{
        int m10;
        int m01;
        int m00;
        Moment() : m10(0),m01(0),m00(0){}
        void reset(){
            m10 = 0;
            m01 = 0;
            m00 = 0;
        }
    };

    void update();
    void extractMoments(Event *e);
    void calcMoments();
    void calcCountour();
    void updateState(int ts);

    RingBuffer<int> *eventsPerInterval;
    RingBuffer<Moment> *moments;
};

#endif // CLUSTER_H
