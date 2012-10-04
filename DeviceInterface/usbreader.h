#ifndef USBREADER_H
#define USBREADER_H

#include "stdafx.h"
#include "ringbuffer.h"

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

};

#endif // USBREADER_H
