#include "cluster.h"
#include <math.h>
#include <algorithm>

#define MAX_AGE_MOMENT 5000 //5000 usec?
#define MAX_AGE_ACTIVITY 10000 //usec
#define NUM_TIMESLOTS 100
#define TIME_WINDOW 1000 //usec
#define PI 3.14159265359

Cluster::Cluster(){
    events = new RingBuffer<Event>();
    eventsPerInterval = new RingBuffer<int>(NUM_TIMESLOTS);

    posX = lastPosX = -1;
    posY = lastPosY = -1;
    lastPolarity = -1;
    firstEventTS = 0;
    lastEventTS = 0;
    candidate = true;
    temporalPredictor = -1; // next occurance of events
    transitionHistory = 0;
    velocity = -1;
    eventCount = 0;

    Event e;
    for(int i = 0; i < events->size; i++){
        events->add(e);
    }
    for(int i = 0; i < NUM_TIMESLOTS; i++){
        eventsPerInterval->add(0);
    }
}

Cluster::~Cluster(){
    delete events;
    delete eventsPerInterval;
    if(transitionHistory)
        delete transitionHistory;
}

void Cluster::addEvent(Event e){
    // add to activity history (eventsPerInterval)
    int lastEventIndex = (lastEventTS/TIME_WINDOW)%NUM_TIMESLOTS;
    int index = (e.timeStamp/TIME_WINDOW)%NUM_TIMESLOTS;
    if(lastEventIndex == index){    // if belonging to same interval, add up
        int sum = eventsPerInterval->at(index);
        eventsPerInterval->set(index,sum+1);
    }
    else{   //else reset to 0 + 1
        eventsPerInterval->set(index,1);
    }

    //add event to cluster
    events->add(e);
    lastPolarity = e.polarity;
    if(firstEventTS == 0)
        firstEventTS = e.timeStamp;
    lastEventTS = e.timeStamp;
    lifeTime = e.timeStamp - firstEventTS;

    calcMoments();
    /*
    current/next polarity ?
    */
}

//calculates centroid
void Cluster::calcMoments(){
    int M00 = 0;  // Area of the cluster/ number of events in cluster
    float M10,M01,M20,M02;
    M10 = M01 = M20 = M02 = 0;

    int i = events->latest;
    while((events->at(events->latest).timeStamp - events->at(i).timeStamp) < MAX_AGE_MOMENT && M00 < events->size){
        M10 += events->at(i).posX;
        M01 += events->at(i).posY;
        M20 += pow(float(events->at(i).posX),2);
        M02 += pow(float(events->at(i).posY),2);

        i--; // go back in time through ringbuffer
        if(i < 0){
            i = events->size-1;
        }
        M00++;
    }
    //printf("#cluster area: %d \r",M00);

    //centroid/ cluster position
    posX = M10/float(M00);
    posY = M01/float(M00);

    //contour -------------------------------- RECHECK!
    float varianceX = M20 - posX*M10;
    float varianceY = M02 - posY*M01;
    float sqrtM00 = sqrt(float(M00));

    float tmp = sqrt(varianceX)/sqrtM00 * 2;
    contourX = (1 > tmp) ? 1 : tmp;

    tmp = sqrt(varianceY)/sqrtM00 * 2;
    contourY = (1 > tmp) ? 1 : tmp;
}

float Cluster::getActivity(){
    //    int numEvents = 0;
    //    int i = events->latest;
    //    while(lastOverallEventTS - events->at(i).timeStamp < MAX_AGE_ACTIVITY && numEvents < events->size){
    //        i--; // go back in time through ringbuffer
    //        if(i < 0){
    //            i = events->size-1;
    //        }
    //        numEvents++;
    //    }
    //    if(numEvents > 0)
    //        activity = float(MAX_AGE_ACTIVITY/numEvents);
    int sum = 0;
    for(int i = 0; i < eventsPerInterval->size; i++){
        sum += eventsPerInterval->at(i);
    }
    float activity = sum/(contourX*contourY*PI);
    return activity;
}

void Cluster::updateTS(int ts){
    // update latest timestamp and lifetime
    lifeTime = ts - firstEventTS;

    //update activity timewindow
    int lastEventIndex = (lastEventTS/TIME_WINDOW)%NUM_TIMESLOTS;
    int tsIndex = (ts/TIME_WINDOW)%NUM_TIMESLOTS;
    if(lastEventIndex!=tsIndex){
        int numSlots = 0;   //number of slots to overwrite
        if(lastEventIndex > tsIndex)
            numSlots = NUM_TIMESLOTS - lastEventIndex + tsIndex;
        else{
            numSlots = tsIndex - lastEventIndex;
        }
        int index = lastEventIndex + 1;
        for(int i = 0; i < numSlots;i++){
            if(index > NUM_TIMESLOTS - 1)
                index = 0;
            eventsPerInterval->set(index,0);
            index++;
        }
    }
}

bool Cluster::isCandidate(){
    return candidate;
}

//merge the events and lifetime of two clusters - NOT NEEDED!
//void Cluster::merge(Cluster *c){
//    if(!this->isCandidate() && !c->isCandidate())
//        return;
//    //update firstEventTS and lifetime
//    if(firstEventTS > c->firstEventTS){
//        updateTS(c->firstEventTS);
//    }

//    //generate new event buffer
//    RingBuffer<Event*> *tmp = new RingBuffer<Event*>();
//    tmp->latest = tmp->size-1;  //set latest timestamp index
//    int counter = 0;
//    int i,j;
//    i = events->latest;
//    j = c->events->latest;
//    while(counter < events->size){
//        if(events->at(i)->timeStamp > c->events->at(j)->timeStamp){ // if this event is newer
//            tmp->set(((tmp->size-1)-counter), events->at(i));
//            i--;
//            if(i < 0){
//                i = events->size-1;
//            }
//        }
//        else{
//            tmp->set(((tmp->size-1)-counter), c->events->at(j));
//            j--;
//            if(j < 0){
//                j = c->events->size-1;
//            }
//        }
//        counter++;
//    }
//    delete events;
//    events = tmp;

//    //if one is a feature cluster, make the new cluster a feature cluster
//    if(!candidate || !c->candidate){
//        candidate = false;
//    }

//    //update cluster position (central moment)
//    calcMoments();
//}
