#include "cluster.h"

Cluster::Cluster(){
    events = new RingBuffer<Event*>();
    posX = -1;
    posY = -1;
    avgTime = -1;
    lastPolarity = -1;
    activity = 0;
}

Cluster::~Cluster(){
    delete events;
}

void Cluster::addEvent(Event *e){
    events->add(e);
    if(lastPolarity == -1)
        lastPolarity = e->polarity;
    lastEventTS = e->timeStamp;
    update(e);
}

//calculates centroid over all events
void Cluster::update(){
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
    avgTime = sumT/numEvents;
}

//calculates centroid over events using last mean
void Cluster::update(Event *e){
    int prevNumEvents = events->size-1;
    posX = (posX * prevNumEvents + e->posX)/events->size;
    posY = (posY * prevNumEvents + e->posY)/events->size;
    avgTime = (avgTime * prevNumEvents + e->timeStamp)/events->size;
}

void Cluster::calcCentralMoment(Event *e){
    int prevNumEvents = events->size-1;
    posX = (posX * prevNumEvents + e->posX)/events->size;
    posY = (posY * prevNumEvents + e->posY)/events->size;
    avgTime = (avgTime * prevNumEvents + e->timeStamp)/events->size;
}

void Cluster::calcCountour(){

}
