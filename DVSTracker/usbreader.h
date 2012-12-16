#ifndef USBREADER_H
#define USBREADER_H

#include "stdafx.h"
#include "event.h"
#include "logger.h"
#include "ringbuffer.h"

class USBReader : public CUsbIoReader
{
public:
    USBReader();
    ~USBReader();
    RingBuffer<Event>* getEventBuffer();

protected:
    virtual void ProcessData(CUsbIoBuf* Buf);

private:
    void readDVS128Event(const char* data, int numBytes);
    RingBuffer<Event> *eventBuffer;
    unsigned int mileStone;

    //Logger *logger;
};

#endif // USBREADER_H
