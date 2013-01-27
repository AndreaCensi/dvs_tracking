#ifndef EVENTPACKET_H
#define EVENTPACKET_H

#include "event.h"

//! A container class events
/*!
    Events arrive in chunks of several. Some of the processing is not needed on a per event basis and would be too costly.
    Thus it is done on per packet basis.
    \author Jonas Strubel
*/
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
