#ifndef PACKETBUFFER_H
#define PACKETBUFFER_H

#include "eventpacket.h"


//! A buffer class for event packets
/*!
    \author Jonas Strubel
*/
class PacketBuffer
{
public:
    PacketBuffer(int bufferLength, int packetSize);
    ~PacketBuffer();
    EventPacket* getNextReadable();
    EventPacket* getNextWritable();
    bool hasNewData();

private:
    EventPacket **packetBuffer;
    int length;
    int start;
    int end;
};

#endif // PACKETBUFFER_H
