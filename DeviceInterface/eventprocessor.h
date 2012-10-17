#ifndef EVENTPROCESSOR_H
#define EVENTPROCESSOR_H

#include "eventprocessorbase.h"
#include "event.h"
#include "ringbuffer.h"
#include "cluster.h"
#include <vector>
#include "camwidget.h"
#include "signalwrapper.h"

class EventProcessor : public EventProcessorBase
{
public:
    EventProcessor();
    ~EventProcessor();
    virtual void processEvent(Event *e);
    SignalWrapper *getSignalWrapper();

private:
    float distance(Event *e, Cluster *c);
    float getBoltzmanWeight(Event *e, Cluster *c);
    void assignToCluster(Event *e);

    std::vector<Cluster*> clusters;
    SignalWrapper *sig;
};

#endif // EVENTPROCESSOR_H
