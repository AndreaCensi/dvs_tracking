#ifndef USBINTERFACE_H
#define USBINTERFACE_H

#include "stdafx.h"
#include "usbreader.h"

class USBInterface
{
public:
    USBInterface(/*void (*process)(unsigned char*)*/);
    ~USBInterface();
    void startReader(int devIndex);
    void sendVendorRequest(CUsbIo dev, UCHAR req);
    short queryDevice();
private:
    void (*processEvent)(unsigned char*);
    HDEVINFO devList;
};

#endif // USBINTERFACE_H
