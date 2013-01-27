#ifndef USBREADER_H
#define USBREADER_H

#include "stdafx.h"
#include "event.h"
#include "logger.h"
#include "ringbuffer.h"
#include "packetbuffer.h"

//! The USB reader thread for the DVS128 camera
/*!
    Inherits from the usb reader thread of Thesycons usbiolib
    \author Jonas Strubel
*/
class USBReader : public CUsbIoReader
{
public:
    USBReader(int usbFifoSize);
    ~USBReader();
    RingBuffer<Event>* getEventBuffer();
    PacketBuffer* getPacketBuffer();

protected:
    virtual void ProcessData(CUsbIoBuf* Buf);

private:
    void readDVS128Event(const char* data, int numBytes);
    PacketBuffer *packetBuffer;
    RingBuffer<Event> *eventBuffer;
    unsigned int mileStone;
};

#endif // USBREADER_H
