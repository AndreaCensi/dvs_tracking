#ifndef EVENTPROCESSOR_H
#define EVENTPROCESSOR_H

#include "eventprocessorbase.h"
#include "event.h"
#include "ringbuffer.h"
#include "cluster.h"
#include <list>
#include "camwidget.h"
#include <QImage>

class EventProcessor : public EventProcessorBase
{
public:
    EventProcessor();
    ~EventProcessor();
    virtual void processEvent(Event *e);
    QImage *getImage();

private:
    void updateImage(Event *e);
    float distance(Event *e, Cluster *c);
    float distance(Event *e1, Event *e2);
    float getBoltzmanWeight(Event *e, Cluster *c);
    void assignToCluster(Event *e);
    //Event* checkWaitingList();

    Event *onMap;
    Event *offMap;
    std::list<Cluster*> clusters;
    QImage * img;
};

#endif // EVENTPROCESSOR_H
