#include "tracker.h"
#include "stdio.h"
#include "localmaximum.h"
#include <math.h>

#define DVS_RES 128

//parameteres
#define SIGMA 0.002f // +- random value -- STILL TO BE CHOSEN!

Tracker::Tracker(std::vector<int> frequencies){
    // Visual output
    camWidget = new CamWidget();
    camWidget->show();
    exit = false;

    //Members
    targetFrequencies = frequencies;

    // init maps
    latestEvents = new Map<Event>(DVS_RES,DVS_RES);
    npTransitions = new Map<Transition>(DVS_RES,DVS_RES);
    pnTransitions = new Map<Transition>(DVS_RES,DVS_RES);

    weightBuffers = new FrequencyAccumulator*[targetFrequencies.size()];

    for(unsigned int i = 0; i < targetFrequencies.size();i++){
        weightBuffers[i] = new FrequencyAccumulator(
                    targetFrequencies[i],SIGMA,5,1.0f,8,5.0f,128,128);
    }
}

Tracker::~Tracker(){
    delete camWidget;
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
        printf("No transition found\n");
        return;
    }

//    printf("Transition recorded at: %f\n",t.timeStamp);

    // Get interval to last transition
    Interval dt = getInterval(t);
    if(dt.timeStamp == 0){
        printf("No interval found\n");
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
        int size;
        if((size = getEventBuffer()->available()) > 0){
            Event *e;
            while((e = getEventBuffer()->getNext()) != 0){
                // do not process if special event
                if(e->isSpecial())
                    return;

                //process events here
                camWidget->updateImage(e);
                processEvent(*e);
            }
        }
        else
            msleep(1);
    }
}
