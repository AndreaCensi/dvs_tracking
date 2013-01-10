#ifndef EVENTPACKET_H
#define EVENTPACKET_H

#include "event.h"

class EventPacket
{
public:
    EventPacket(int maxLength);
    ~EventPacket();
    void add(Event e);
    Event* get(int i);
    int size();
    void reset();

private:
    Event *buffer;
    int length;
    int index;

};

#endif // EVENTPACKET_H
