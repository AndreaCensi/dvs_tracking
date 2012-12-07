#ifndef CLUSTER_H
#define CLUSTER_H

#include "ringbuffer.h"
#include "event.h"
#include "transitionhistory.h"
#include "vector2d.h"

class Cluster
{
public:
    Cluster();
    ~Cluster();
    void addEvent(Event e);
    bool isCandidate();
    void update(int ts);
    float getActivity();
    void convert(); //convert to candidate to feature cluster

    RingBuffer<Event> *events;

    float posX;
    float posY;

    int currentState;
    int lifeTime;

    Vector2D velocity;

    bool assigned;

    unsigned int firstEventTS;  //first event assigned to this cluster
    unsigned int lastEventTS;    // latest event timestamp from camera, used for liftime and activity comparison
    unsigned int lastUpdateTS;

    float contourX;
    float contourY;
    bool candidate;

    TransitionHistory *transitionHistory;

private:
    struct Moment{
        int m10;
        int m01;
        int m00;
        float m20;
        float m02;

        void reset(){
            m10 = 0;
            m01 = 0;
            m00 = 0;
            m20 = 0;
            m02 = 0;
        }

        Moment(){
            reset();
        }
    };

    struct Position{
        float x;
        float y;
        int timestamp;

        void reset(){
            x = 0;
            y = 0;
            timestamp = 0;
        }

        Position(){
            reset();
        }
    };

    struct Acceleration{
        float m;
        float a;

        void reset(){
            m = 0;
            a = 0;
        }

        Acceleration(){
            reset();
        }
    };

    void extractMoments(Event *e);
    void updateState(int ts);
    void updatePath(int ts);
    void updateVelocity();
    void predictRelativePosition();

    RingBuffer<int> *eventsPerInterval;
    RingBuffer<Moment> *moments;
    RingBuffer<Position> *path;

    Vector2D estimatedVelocity;
    Acceleration acceleration;
};

#endif // CLUSTER_H
