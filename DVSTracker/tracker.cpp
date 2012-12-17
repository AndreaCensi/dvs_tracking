#include "tracker.h"
#include "stdio.h"
#include "localmaximum.h"
#include <math.h>

#define DVS_RES 128

//parameteres
#define SIGMA_W 0.0002f // +- random value -- STILL TO BE CHOSEN!
#define FILTER_SIZE 3
#define SIGMA_FILTER 0.75f
#define MIN_DIST 4.0f
#define NUM_MAXIMA 8

Tracker::Tracker(RingBuffer<Event> *buffer, std::vector<int> frequencies, QObject *parent) : QThread(parent){
    //Members
    eventBuffer = buffer;
    targetFrequencies = frequencies;

    widget = 0;

    exit = false;

    // init maps
    latestEvents = new Map<Event>(DVS_RES,DVS_RES);
    npTransitions = new Map<Transition>(DVS_RES,DVS_RES);
    pnTransitions = new Map<Transition>(DVS_RES,DVS_RES);

    weightBuffers = new FrequencyAccumulator*[targetFrequencies.size()];

    for(unsigned int i = 0; i < targetFrequencies.size();i++){
        weightBuffers[i] = new FrequencyAccumulator(
                    targetFrequencies[i],SIGMA_W,FILTER_SIZE,SIGMA_FILTER,MIN_DIST,NUM_MAXIMA,DVS_RES,DVS_RES);
    }
}

Tracker::~Tracker(){
    delete latestEvents;
    delete npTransitions;
    delete pnTransitions;

    for(unsigned int i = 0; i < targetFrequencies.size();i++){
        delete weightBuffers[i];
    }
    delete [] weightBuffers;
}

void Tracker::processEvent(Event e){
    // Record, if there is a transition
    Transition t = getTransition(e);
    if(t.timeStamp == 0){
        //        printf("No transition found\n");
        return;
    }

    // Get interval to last transition
    Interval dt = getInterval(t);
    if(dt.timeStamp == 0 || dt.deltaT < 0){
        //printf("No interval found\n");
        return;
    }

    //    printf("Interval recorded with dt, ts: %f,%f\n",dt.deltaT,dt.timeStamp);

    //Calculate importance of interval for each frequency
    for(unsigned int i = 0; i < targetFrequencies.size(); i++){
        FrequencyAccumulator *buf = weightBuffers[i];
        buf->update(dt);
        std::vector<LocalMaximum> maxima;
        if(buf->hasExpired()){
            maxima = buf->evaluate();
            updateWeightWidget(i,buf);
            buf->reset();
        }
        if(maxima.size() > 0){
            // continue processing
        }
    }

}

Transition Tracker::getTransition(Event e){
    //Get last event at same position and overwrite with new event.
    Event last = latestEvents->get(e.x,e.y);
    latestEvents->insert(e.x,e.y,e);
    if(last.timeStamp == 0)
        return Transition(0);
    //If consecutive events differ, create and return a transition
    if(last.type != e.type)
        return Transition(e.timeStamp,e.x,e.y,e.type);
    else
        return Transition(0);
}

Interval Tracker::getInterval(Transition t){
    Map<Transition> *transitions = (t.type == 1) ? npTransitions : pnTransitions;
    Transition last = transitions->get(t.x,t.y);
    transitions->insert(t.x,t.y,t);

    if(last.timeStamp == 0)
        return Interval(0);
    else{
        double deltaT = t.timeStamp - last.timeStamp;
        return Interval(t.timeStamp,t.x,t.y,deltaT);
    }
}

void Tracker::stop(){
    exit = true;
}

void Tracker::run(){
    while(!exit){
        if((eventBuffer->available()) > 0){

            //updateCamWidget(eventBuffer->latestIndex(),eventBuffer->available());

            Event *e;
            while((e = eventBuffer->getNext()) != 0){
                // do not process if special event
                if(e->isSpecial())
                    return;

                //process events here
                //updateCamWidget(e);
                processEvent(*e);
            }
        }
        else
            msleep(10);
    }
}

void Tracker::setWidget(CamWidget *camWidget){
    widget = camWidget;
}

void Tracker::updateCamWidget(Event *e){
    if(widget == 0)
        return;
    widget->updateImage(e);
}

void Tracker::updateWeightWidget(int i, FrequencyAccumulator *buf){
    for(int y = 0; y < DVS_RES;y++){
        for(int x = 0; x < DVS_RES;x++){
            float value = buf->weightMap->get(x,y);
//            if(value > 0)
//                printf("%f\n",value);
            int grey = int(value/4.0);
            if(grey > 255)
                grey = 255;
            if(grey > 0){
                widget->updateImage(x,y,grey);
            }
        }
    }
}
