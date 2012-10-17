#ifndef USBINTERFACE_H
#define USBINTERFACE_H

#include "stdafx.h"
#include "usbreader.h"
#include "eventprocessorbase.h"

class USBInterface
{
public:
    USBInterface(EventProcessorBase *ep);
    ~USBInterface();
    void startReading();
    void stopReading();
    void sendVendorRequest(UCHAR req, const char *buf = NULL, DWORD bufSize = 0);
    USBReader *getReaderInstance();

private:
    void startReaderThread(int devIndex);
    USBReader *reader;
    short devIndex;

};

#endif // USBINTERFACE_H
