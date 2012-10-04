#ifndef USBINTERFACE_H
#define USBINTERFACE_H

#include "stdafx.h"
#include "usbreader.h"

class USBInterface
{
public:
    USBInterface();
    ~USBInterface();
    void startReader(int devIndex);
    void sendVendorRequest(CUsbIo dev, UCHAR req);
    short queryDevice();
private:
//    GUID usbIoID;
    HDEVINFO devList;
    //USB_DEVICE_DESCRIPTOR devDesc;
    //    DWORD devStatus;
    //    bool DevFound;

    //    struct DeviceConfig{
    //        unsigned short vid;
    //        unsigned short pid;
    //        unsigned char endpoint;
    //        unsigned short fifoSize;
    //        unsigned short cfgIndex;
    //        unsigned short cfgNumInterfaces;
    //        unsigned short cfgInterface;
    //        unsigned short cfgAltSetting;
    //        unsigned long cfgMaxTransfer;
    //    };

    //    DeviceConfig devCfg;

};

#endif // USBINTERFACE_H
