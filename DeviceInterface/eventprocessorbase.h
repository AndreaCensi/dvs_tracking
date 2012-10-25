#ifndef EVENTPROCESSORBASE_H
#define EVENTPROCESSORBASE_H

#include "event.h"

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
