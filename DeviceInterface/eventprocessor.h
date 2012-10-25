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
    virtual void processEvent(Event e);
    QImage* getImage();
    std::vector<Cluster*>* getClusters();

private:
    // Graphical output
    void updateImage(Event *e);

    // Cost-function
    float distance(Event *e, Cluster *c);
    float distanceToContour(Event *e, Cluster *c);
    float distance(Event *e1, Event *e2);
    float distance(Cluster *c1, Cluster *c2);
    float getBoltzmanWeight(Event *e, Cluster *c);
    float getSpatioTemporalCost(Event *e, Cluster *c);
    float boundaryCost(Event *e, Cluster *c);
    float temporalCost(Event *e, Cluster *c);
    float cumulativeDistribution(float l, float x);

    // Assignement
    void updateMap(Event e);
    void assignToCluster(Event e);
    //void mapAssign(Event *e);
    std::vector<Event> labelingFilter(Event e);

    //maintanance
    void maintainClusters();

    Event *onMap;
    Event *offMap;

    std::vector<Event*> clusterCandidates;
    std::vector<Cluster*> candidateClusters;
    std::vector<Cluster*> clusters;
    QImage * img;
};

#endif // EVENTPROCESSOR_H
