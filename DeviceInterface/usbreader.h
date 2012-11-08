#ifndef USBREADER_H
#define USBREADER_H

#include "stdafx.h"
#include "eventprocessorbase.h"
#include "event.h"
#include "logger.h"

class USBReader : public CUsbIoReader
{
public:
    USBReader(EventProcessorBase *ep);
    ~USBReader();

protected:
    virtual void ProcessData(CUsbIoBuf* Buf);

private:
    void readDVS128Event(const char* data, int numBytes);
    unsigned int mileStone;
    EventProcessorBase *eventProcessor;
    Event eventBuffer[128];
    Logger *logger;
};

#endif // USBREADER_H
