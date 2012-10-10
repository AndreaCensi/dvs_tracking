#ifndef EVENTREADER_H
#define EVENTREADER_H

#include "event.h"

class EventReader
{
public:
    EventReader();
    ~EventReader();
    static void processEvent(Event event);
};

#endif // EVENTREADER_H
