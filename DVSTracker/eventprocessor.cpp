#include "eventprocessor.h"
#include "stdio.h"
#include <math.h>

//other
#define DVS_RES 128

EventProcessor::EventProcessor(){
    camWidget = new CamWidget();
    camWidget->show();
    exit = false;
}

EventProcessor::~EventProcessor(){
    delete camWidget;
}

void EventProcessor::processEvent(Event e){

}

void EventProcessor::stop(){
    exit = true;
}

void EventProcessor::run(){
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
            }
        }
        else
            msleep(1);
    }
}
