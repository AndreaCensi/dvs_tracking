#ifndef EVENTPROCESSOR_H
#define EVENTPROCESSOR_H

#include "eventprocessorbase.h"
#include "event.h"
#include "ringbuffer.h"
#include "cluster.h"
#include <vector>
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
    float getBoltzmanWeight(Event *e, Cluster *c);
    void assignToCluster(Event *e);    
    std::vector<Cluster*> clusters;
    QImage * img;
};

#endif // EVENTPROCESSOR_H
