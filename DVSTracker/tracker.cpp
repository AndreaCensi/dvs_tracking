#include "tracker.h"
#include "stdio.h"
#include <math.h>

#define DVS_RES 128

//parameteres
#define SIGMA 50 //random value -- STILL TO BE CHOSEN!

Tracker::Tracker(std::vector<int> frequencies){
    // Visual output
    camWidget = new CamWidget();
    camWidget->show();
    exit = false;


    //Members
    targetFrequencies = frequencies;

    // init maps
    latestEvents = new Map<Event>[DVS_RES*DVS_RES];
    npTransitions = new Map<Transition>[DVS_RES*DVS_RES];
    pnTransitions = new Map<Transition>[DVS_RES*DVS_RES];

    for(int i = 0; i < targetFrequencies.size();i++){
        FrequencyAccumulator fa;
        weightBuffers.push_back();
    }

}

Tracker::~Tracker(){
    delete camWidget;
    delete latestEvents;
    delete npTransitions;
    delete pnTransitions;

}

void Tracker::processEvent(Event e){
    // Record, if there is a transition
    Transition t = getTransition(e);
    if(t.timeStamp == 0)
        return;

    // Get interval to last transition
    Interval dt = getInterval(t);
    if(dt.timeStamp == 0)
        return;

    // Calculate importance of interval for each frequency
    for(int i = 0; i < weightBuffers.size(); i++){

    }

}

Transition Tracker::getTransition(Event e){
    //Get last event at same position and overwrite with new event.
    Event last = lastEvents->get(e.x,e.y);
    lastEvents->insert(e.x,e.y,e);

    if(last == 0)
        return;
    //If consecutive events differ, create and return a transition
    if(last.type != e.type)
        return Transition(e.timeStamp,e.x,e.y,e.type);
    else
        return Transition(0);
}

void Tracker::getInterval(Transition t){
    Map<Transition> *transitions = (t.type == 1) ? npTransitions : pnTransitions;
    Transition last = transitions->get(t.x,t.y);
    transitions->insert(t.x,t.y,t);

    if(last == 0)
        return;

    double deltaT = t.timeStamp - last.timeStamp;
    return Interval(t.timeStamp,t.x,t.y,deltaT);
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
                processEvent(e);
            }
        }
        else
            msleep(1);
    }
}
