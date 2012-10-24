#include "cluster.h"

#define MAX_AGE_MOMENT 5000 //usec
#define MAX_AGE_ACTIVITY 1000.0 //usec

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

    calcCentralMoment();

    /*
    current/next polarity ?
    */
}

//calculates centroid
void Cluster::calcCentralMoment(){
    int numEvents = 0;
    int sumX,sumY;
    sumX = sumY = 0;

    int i = events->latest;
    while( events->at(i) != 0 && (lastOverallEventTS - events->at(i)->timeStamp < MAX_AGE_MOMENT || numEvents > events->size)){
        sumX += events->at(i)->posX;
        sumY += events->at(i)->posY;

        i--; // go back in time through ringbuffer
        if(i < 0){
            i = events->size-1;
        }
        numEvents++;
    }
    posX = sumX/numEvents;
    posY = sumY/numEvents;
}

float Cluster::getActivity(){
    int numEvents = 0;
    int i = events->latest;
    while( events->at(i) != 0 && (lastOverallEventTS - events->at(i)->timeStamp < MAX_AGE_ACTIVITY || numEvents > events->size)){
        i--; // go back in time through ringbuffer
        if(i < 0){
            i = events->size-1;
        }
        numEvents++;
    }
    activity = MAX_AGE_ACTIVITY/numEvents;
    return activity;
}

void Cluster::calcCountour(){

}

void Cluster::updateTS(int ts){
    lastOverallEventTS = ts;
    lifeTime = lastOverallEventTS - firstEventTS;
}

bool Cluster::isCandidate(){
    return candidate;
}

//merge the events and lifetime of two clusters - not needed!
void Cluster::merge(Cluster *c){
    if(!this->isCandidate() && !c->isCandidate())
        return;
    //update firstEventTS and lifetime
    if(firstEventTS > c->firstEventTS){
        updateTS(c->firstEventTS);
    }

    //generate new event buffer
    RingBuffer<Event*> *tmp = new RingBuffer<Event*>();
    tmp->latest = tmp->size-1;  //set latest timestamp index
    int counter = 0;
    int i,j;
    i = events->latest;
    j = c->events->latest;
    while(counter < events->size){
        if(events->at(i)->timeStamp > c->events->at(j)->timeStamp){ // if this event is newer
            tmp->set(((tmp->size-1)-counter), events->at(i));
            i--;
            if(i < 0){
                i = events->size-1;
            }
        }
        else{
            tmp->set(((tmp->size-1)-counter), c->events->at(j));
            j--;
            if(j < 0){
                j = c->events->size-1;
            }
        }
        counter++;
    }
    delete events;
    events = tmp;

    //if one is a feature cluster, make the new cluster a feature cluster
    if(!candidate || !c->candidate){
        candidate = false;
    }

    //update cluster position (central moment)
    calcCentralMoment();
}
