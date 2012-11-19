#ifndef FILTER_H
#define FILTER_H

#include "event.h"

class Filter
{
public:
    Filter(int res = 128);
    ~Filter();
    Event* labelingFilter(Event e);
    int availableEvents();
    int *getHistogram();

private:
    void updateMap(Event e);
    bool checkHistogram(Event *e);
    void reset();

    int resolution;

    int lastAgeingTS;

    int index;
    bool empty;
    Event *candidates;

    Event *onMap;
    Event *offMap;

    int *isiHistogram;
    int sumHistogram;
};

#endif // FILTER_H
