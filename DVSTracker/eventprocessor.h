#ifndef EVENTPROCESSOR_H
#define EVENTPROCESSOR_H

#include "eventprocessorbase.h"
#include "event.h"
#include "ringbuffer.h"
#include "camwidget.h"

class EventProcessor : public EventProcessorBase
{
public:
    EventProcessor();
    ~EventProcessor();
    virtual void processEvent(Event e);
    void run();
    void stop();

private:
    //graphical output
    CamWidget *camWidget;

    bool exit;
};

#endif // EVENTPROCESSOR_H
