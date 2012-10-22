#include "cluster.h"

#define MAX_AGE_MOMENT 5000 //usec
#define MAX_AGE_ACTIVITY 1000 //usec

Cluster::Cluster(){
    events = new RingBuffer<Event*>();
    posX = lastPosX = -1;
    posY = lastPosY = -1;
    lastPolarity = -1;
    activity = 0;
    firstEventTS = 0;
    activity = 0;
    candidate = true;
}

Cluster::~Cluster(){
    delete events;
}

void Cluster::addEvent(Event *e){
    events->add(e);
    lastPolarity = e->polarity;
    if(firstEventTS == 0)
        firstEventTS = e->timeStamp;
    lastOverallEventTS = e->timeStamp;
    lifeTime = lastOverallEventTS - firstEventTS;

    getCentralMoment();
    getActivity();
    /*
    activity
    current/next polarity ?
    */
}

//calculates centroid
void Cluster::getCentralMoment(){
    int numEvents = 0;
    int sumX,sumY;
    sumX = sumY = 0;
    Event *buffer = *events->buffer;

    int index = events->latest;
    while(lastEventTS - events->at(index)->timeStamp < MAX_AGE_MOMENT || numEvents > events->size){
        sumX += buffer[i].posX;
        sumY += buffer[i].posY;

        index--; // go back in time through ringbuffer
        if(index < 0){
            index = events->size-1;
        }
        numEvents++;
    }
    posX = sumX/numEvents;
    posY = sumY/numEvents;
}

float Cluster::getActivity(){
    int numEvents = 0;
    int index = events->latest;
    while(lastOverallEventTS - events->at(index)->timeStamp < MAX_AGE_ACTIVITY || numEvents > events->size){
        index--; // go back in time through ringbuffer
        if(index < 0){
            index = events->size-1;
        }
        numEvents++;
    }
    activity = MAX_AGE_ACTIVITY/numEvents;
    return activity;
}

void Cluster::getCountour(){

}

void Cluster::updateTS(int ts){
    lastOverallEventTS = ts;
    lifeTime = lastOverallEventTS - firstEventTS;
}

bool Cluster::isCandidate(){
    return candidate;
}
