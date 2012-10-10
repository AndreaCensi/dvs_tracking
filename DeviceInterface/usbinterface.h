#ifndef USBINTERFACE_H
#define USBINTERFACE_H

#include "stdafx.h"
#include "usbreader.h"

class USBInterface
{
public:
    USBInterface(void (*process)(Event event));
    ~USBInterface();
    void startReading();
    void stopReading();
    void sendVendorRequest(UCHAR req, char *buffer);

private:
    void startReaderThread(int devIndex);
    USBReader *reader;
    short devIndex;

};

#endif // USBINTERFACE_H
