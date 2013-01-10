#include "packetbuffer.h"

PacketBuffer::PacketBuffer(int bufferLength, int packetSize)
{
    packetBuffer = new EventPacket*[bufferLength];
    for(int i = 0; i < bufferLength;i++)
        packetBuffer[i] = new EventPacket(packetSize);

    length = bufferLength;
    start = 0;
    end = 0;
}

PacketBuffer::~PacketBuffer(){
    for(int i = 0; i < length;i++)
        delete packetBuffer[i];
    delete [] packetBuffer;
}

EventPacket* PacketBuffer::getNextReadable(){
    if(start != end){
        EventPacket *next = packetBuffer[start];
        start++;
        if(start == length)
            start = 0;
        return next;
    }
    else return 0;
}

EventPacket* PacketBuffer::getNextWritable(){
    EventPacket *next = packetBuffer[end];
    end++;
    if(end == length)
        end = 0;
    return next;
}

bool PacketBuffer::hasNewData(){
    if(start != end)
        return true;
    else
        return false;
}
