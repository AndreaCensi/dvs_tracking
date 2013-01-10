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
#include "particlefilter.h"
#include "combinationanalyzer.h"
#include "packetbuffer.h"
#include <vector>


class Tracker : public QThread
{
    Q_OBJECT
public:
    Tracker(PacketBuffer *buffer, std::vector<int> frequencies, QObject *parent = 0);
    ~Tracker();
    void setWidget(CamWidget *camWidget);
    void run();
    void stop();

    FrequencyAccumulator **weightBuffers;
    ParticleFilter **particleFilters;

private:
    Transition getTransition(Event e);
    Interval getInterval(Transition t);
    void processEvent(Event e);
    void processPacket();

    //    RingBuffer<Event> *eventBuffer;
    PacketBuffer *packetBuffer;

    CamWidget *widget;

    //storage
    Map<Event> *latestEvents;
    Map<Transition> *pnTransitions;
    Map<Transition> *npTransitions;

    std::vector<int> targetFrequencies;

    bool exit;

    //graphical output
    void updateCamWidget(Event *e);
    void updateWeightWidget(int bufID, FrequencyAccumulator *buf, Maxima *m);

    HypothesisLogger *logger;
    double lastEventTs;
    int eventCount;

    //evaluation
    CombinationAnalyzer *combinationAnalyzer;

signals:
    void updateCamWidget(int,int);
};

#endif // EVENTPROCESSOR_H
