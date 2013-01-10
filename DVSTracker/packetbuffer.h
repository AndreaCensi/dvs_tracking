#ifndef PACKETBUFFER_H
#define PACKETBUFFER_H

#include "eventpacket.h"

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
