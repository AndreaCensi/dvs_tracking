#ifndef USBINTERFACE_H
#define USBINTERFACE_H

#include "stdafx.h"
#include "usbreader.h"

//! The interface class to the DVS128 camera
/*!
    \author Jonas Strubel
*/
class DVS128Interface
{
public:
    DVS128Interface();
    ~DVS128Interface();
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
