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

void Cluster::addEvent(Event event){
    events->add(event);
    if(polarity == -1)
        polarity = event.polarity;
    update();
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
