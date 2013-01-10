#include "eventpacket.h"

EventPacket::EventPacket(int bufferLength)
{
    length = bufferLength;
    buffer = new Event[length];
    reset();
}

EventPacket::~EventPacket(){
    delete [] buffer;
}

void EventPacket::add(Event e){
    buffer[index] = e;
    index++;
}

Event* EventPacket::get(int i){
    return &buffer[i];
}

int EventPacket::size(){
    return index;
}

void EventPacket::reset(){
    index = 0;
}
