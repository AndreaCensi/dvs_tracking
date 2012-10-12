#ifndef USBREADER_H
#define USBREADER_H

#include "stdafx.h"
#include "ringbuffer.h"
#include "camwidget.h"
#include "event.h"
#include <QUdpSocket>
#include "signalwrapper.h"

class USBReader : public CUsbIoReader
{
public:
    USBReader(void (*process)(Event event));
    ~USBReader();

    SignalWrapper *getSignalWrapper();

protected:
    virtual void ProcessData(CUsbIoBuf* Buf);

private:
    void readDVS128Event(const char* data, int numBytes);
    void (*processEvent)(Event event);
    unsigned int mileStone;
    RingBuffer<Event> *events;
    SignalWrapper *sig;
    QUdpSocket sock;
};

#endif // USBREADER_H
