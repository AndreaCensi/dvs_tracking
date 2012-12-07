#include "tracker.h"
#include "stdio.h"
#include <math.h>

//other
#define DVS_RES 128
#define NUM_FREQUENCIES 4

Tracker::Tracker(){
    camWidget = new CamWidget();
    camWidget->show();
    exit = false;

    latestEvents = new Map<Event>[DVS_RES*DVS_RES];
    npTransitions = new Map<Transition>[DVS_RES*DVS_RES];
    pnTransitions = new Map<Transition>[DVS_RES*DVS_RES];
}

Tracker::~Tracker(){
    delete camWidget;
    delete [] latestEvents;
    delete [] npTransitions;
    delete [] pnTransitions;
}

void Tracker::processEvent(Event e){
    Transition t = getTransition(e);
    if(t.timeStamp == 0)
        return;
    Interval dt = getInterval(t);
    if(dt.timeStamp == 0)
        return;


}

Transition Tracker::getTransition(Event e){
    //Get last event at same position and overwrite with new event.
    Event last = lastEvents->get(e.x,e.y);
    lastEvents->insert(event);

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
    transitions->insert(t);

    if(last == 0)
        return;

    unsigned int deltaT = t.timeStamp - last.timeStamp;
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
