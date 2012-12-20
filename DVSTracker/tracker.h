#ifndef EVENTPROCESSOR_H
#define EVENTPROCESSOR_H

#include <QThread>
#include "event.h"
#include "ringbuffer.h"
#include "camwidget.h"
#include "transition.h"
#include "interval.h"
#include "map.h"
#include "frequencyaccumulator.h"
#include "camwidget.h"
#include "hypothesislogger.h"
#include <vector>


class Tracker : public QThread
{
    Q_OBJECT
public:
    Tracker(RingBuffer<Event> *buffer, std::vector<int> frequencies, QObject *parent = 0);
    ~Tracker();
    void processEvent(Event e);
    void setWidget(CamWidget *camWidget);
    void run();
    void stop();

    FrequencyAccumulator **weightBuffers;

private:
    Transition getTransition(Event e);
    Interval getInterval(Transition t);

    RingBuffer<Event> *eventBuffer;

    CamWidget *widget;

    //storage
    Map<Event> *latestEvents;
    Map<Transition> *pnTransitions;
    Map<Transition> *npTransitions;

    std::vector<int> targetFrequencies;

    bool exit;

    void updateCamWidget(Event *e);
    void updateWeightWidget(int bufID, FrequencyAccumulator *buf, Maxima *m);
    HypothesisLogger *logger;
    double lastEventTs;
    int eventCount;

signals:
    void updateCamWidget(int,int);
};

#endif // EVENTPROCESSOR_H
