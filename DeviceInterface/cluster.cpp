#include "cluster.h"

Cluster::Cluster(){
    events = new RingBuffer<Event>();
    posX = -1;
    posY = -1;
    avgTime = -1;
    polarity = -1;
}

Cluster::~Cluster(){
    delete events;
}

void Cluster::addEvent(Event e){
    events->add(e);
    if(polarity == -1)
        polarity = e.polarity;
    lastEventTime = e.timeStamp;
    update(e);
}

void Cluster::update(){
    int numEvents = events->size;
    int sumX,sumY,sumT;
    sumX = sumY = sumT = 0;
    Event *buffer = events->buffer;
    for(int i = 0; i < numEvents;i++){
        sumX += buffer[i].posX;
        sumY += buffer[i].posY;
        sumT += buffer[i].timeStamp;
    }
    posX = sumX/numEvents;
    posY = sumY/numEvents;
    avgTime = sumT/numEvents;
}

void Cluster::update(Event e){
    int prevNumEvents = events->size-1;
    posX = (posX * prevNumEvents + e.posX)/events->size;
    posY = (posY * prevNumEvents + e.posY)/events->size;
    avgTime = (avgTime * prevNumEvents + e.timeStamp)/events->size;
}

void Cluster::calcMoment(){

}
