#ifndef FILTER_H
#define FILTER_H

#include "event.h"

class Filter
{
public:
    Filter(int res = 128);
    ~Filter();
    Event* labelingFilter(Event e, int maxTDiff);
    int size();
private:
    void updateMap(Event e);
    void reset();

    int resolution;

    int index;
    bool empty;
    Event *candidates;

    Event *onMap;
    Event *offMap;
};

#endif // FILTER_H
