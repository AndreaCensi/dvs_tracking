#ifndef USBREADER_H
#define USBREADER_H

#include "stdafx.h"
#include "ringbuffer.h"
#include "camwidget.h"
#include "event.h"
#include <QUdpSocket>

class USBReader : public CUsbIoReader
{
public:
    USBReader(void (*process)(Event event));
    ~USBReader();
protected:
    virtual void ProcessData(CUsbIoBuf* Buf);
private:
    void readDVS128Event(const char* data, int numBytes);
    void (*processEvent)(Event event);
    unsigned int mileStone;
    RingBuffer<Event> *events;
};

#endif // USBREADER_H
