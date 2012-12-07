#ifndef EVENTPROCESSOR_H
#define EVENTPROCESSOR_H

#include "eventprocessorbase.h"
#include "event.h"
#include "ringbuffer.h"
#include "camwidget.h"
#include "transition.h"
#include "interval.h"
#include "map.h"

class Tracker : public EventProcessorBase
{
public:
    Tracker();
    ~Tracker();
    virtual void processEvent(Event e);
    void run();
    void stop();

private:
    Transition getTransition(Event e);
    Interval getInterval(Transition t);

    //graphical output
    CamWidget *camWidget;

    //storage
    Map<Event> *latestEvents;
    Map<Transition> *pnTransitions;
    Map<Transition> *npTransitions;

    Map<Transition> *weightedIntervalBuffers;

    bool exit;
};

#endif // EVENTPROCESSOR_H
