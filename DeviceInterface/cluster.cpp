#include "cluster.h"

#define MAX_AGE 1000 //usec

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
    lastEventTS = e->timeStamp;

    calcCentralMoment();
    /*
    central moment
    activity
    lifetime
    lastEventTS
    newPolarity
    */
}

//calculates centroid
void Cluster::calcCentralMoment(){
    int index = events->latest;
    while(lastEventTS - events->at(index)->timeStamp < MAX_AGE){
        int numEvents = events->size;
        int sumX,sumY,sumT;
        sumX = sumY = sumT = 0;
        Event *buffer = *events->buffer;
        for(int i = 0; i < numEvents;i++){
            sumX += buffer[i].posX;
            sumY += buffer[i].posY;
            sumT += buffer[i].timeStamp;
        }
        posX = sumX/numEvents;
        posY = sumY/numEvents;

        index--; // go back in time trough ringbuffer
        if(index < 0){
            index = events->size-1;
        }
    }
}

void Cluster::calcCountour(){

}

bool Cluster::isCandidate(){
    return candidate;
}
