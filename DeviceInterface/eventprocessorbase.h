#ifndef EVENTPROCESSORBASE_H
#define EVENTPROCESSORBASE_H

#include "event.h"
#include "ringbuffer.h"
#include <QThread>

/*
Inherit  from this class and implement processEvent to process incoming events.
A reference to the instance of the child class must be provided to the usbinterface.
*/
class EventProcessorBase : public QThread
{
protected:
    RingBuffer<Event> *eventBuffer;


    EventProcessorBase(){
        eventBuffer = new RingBuffer<Event>();
    }

public:
    ~EventProcessorBase(){
        delete eventBuffer;
    }

    virtual void processEvent(Event e){
    }

    RingBuffer<Event> *getEventBuffer(){
        return eventBuffer;
    }
};

#endif // EVENTPROCESSORBASE_H
