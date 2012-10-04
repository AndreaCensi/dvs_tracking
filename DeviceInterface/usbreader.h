#ifndef USBREADER_H
#define USBREADER_H

#include "stdafx.h"
#include "ringbuffer.h"
#include "udpclient.h"

class USBReader : public CUsbIoReader
{
public:
    USBReader(/*void (*process)(unsigned char*)*/);
    ~USBReader();
protected:
    virtual void ProcessData(CUsbIoBuf* Buf);
private:
    void processEvent(unsigned char* data);
    RingBuffer<unsigned char> *rBuf;
    unsigned int mileStone;
    UDPClient udpClient;
    struct Event{
        unsigned short xAddr;
        unsigned short yAddr;
        unsigned int rawAddr;
        unsigned int timeStamp;
        bool eventType;
    };

};

#endif // USBREADER_H
