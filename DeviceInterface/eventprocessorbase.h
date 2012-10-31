#ifndef EVENTPROCESSORBASE_H
#define EVENTPROCESSORBASE_H

#include "event.h"

/*
Inherit  from this class and implement processEvent to process incoming events.
A reference to the instance of the child class must be provided to the usbinterface.
*/
class EventProcessorBase
{
public:
    EventProcessorBase(){
    }
    virtual ~EventProcessorBase(){
    }
    virtual void processEvent(Event e){
    }
};

#endif // EVENTPROCESSORBASE_H
