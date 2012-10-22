#ifndef EVENTPROCESSOR_H
#define EVENTPROCESSOR_H

#include "eventprocessorbase.h"
#include "event.h"
#include "ringbuffer.h"
#include "cluster.h"
#include <list>
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
    // Graphical output
    void updateImage(Event *e);

    // Cost-function
    float distance(Event *e, Cluster *c);
    float distance(Event *e1, Event *e2);
    float getBoltzmanWeight(Event *e, Cluster *c);

    // Assignement
    void updateMap(Event *e);
    void assignToCluster(Event *e);
    void mapAssign(Event *e);
    std::vector<Event*> labelingFilter(Event *e);

    Event **onMap;
    Event **offMap;

    std::vector<Event*> clusterCandidates;
    std::vector<Cluster*> candidateClusters;
    std::vector<Cluster*> clusters;
    QImage * img;
};

#endif // EVENTPROCESSOR_H
