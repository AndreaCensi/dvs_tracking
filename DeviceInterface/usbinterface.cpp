#include "stdafx.h"
#include "usbinterface.h"
#include <iostream>

#define NUM_BUFFERS 5

//device config
#define  VID 0x152A
#define  PID 0x8400
#define  ENDPOINT 0x86
#define	 ENDPOINT_FIFO_SIZE 512
#define  CFG_INDEX 0
#define  CFG_NUM_INTERFACES 1
#define  CFG_INTERFACE 0
#define  CFG_ALTSETTING 0
#define	 CFG_MAX_TRANSFER 4096

GUID usbIoID = USBIO_IID;
HDEVINFO devList;

USBInterface::USBInterface(/*void (*process)(unsigned char*)*/){
    //processEvent = process;
    devList = NULL;
}

USBInterface::~USBInterface(){

}

short USBInterface::queryDevice(){
    CUsbIo dev;
    USB_DEVICE_DESCRIPTOR devDesc;
    DWORD status;
    bool found = false;
    short index = -1;

    devList = CUsbIo::CreateDeviceList(&usbIoID);
    if (devList == NULL){
        printf("Unable to build a device list!\n");
    }

    // open and query each device (max. 127)
    for (int i = 0; i < 127; i++){
        fprintf(stdout,"i: %d\n",i);
        status = dev.Open(i,devList,&usbIoID);
        if ( status != USBIO_ERR_SUCCESS ) {
            // no more devices, leave loop
            if ( status != USBIO_ERR_NO_SUCH_DEVICE_INSTANCE ) {
                fprintf(stdout,"UsbDev.Open returned with error 0x%08X\n",status);
            }
            fprintf(stdout,"No more devices leaving loop...\n");
            break;
        }
        // we have found a device, query the device descriptor
        status = dev.GetDeviceDescriptor(&devDesc);
        fprintf(stdout,"vid: %x\n",devDesc.idVendor);
        if ( status == USBIO_ERR_SUCCESS ){
            // found one valid device
            found = true;
            fprintf(stdout,"Valid device found...\n");
            // Does the device have a serial number?
            if ( devDesc.iSerialNumber!=0 ){
                // close device
                dev.Close();
                if (devDesc.idVendor == VID && devDesc.idProduct == PID){
                    // start our Reader
                    fprintf(stdout,"Device found, starting Reader.\n");
                    index = i;
                    startReader(i);
                    //break; // leave loop
                } else {
                    fprintf(stdout,"Device not recognized\n");
                }
            } else {
                // "GetDeviceDescriptor" failed, seems not to be a valid USB device.
                fprintf(stdout,"Query Device Descriptor failed, status:0x%08X\n",status);
                // close device
                dev.Close();
            }
        } // for

        if ( !found ) {
            // no device found
            fprintf(stdout,"There are no USB devices attached to the USBIO driver.\n");
        }
        else
            fprintf(stdout,"Device found...");

        // free device list
        CUsbIo::DestroyDeviceList(devList);
    }
    return index;
}

void USBInterface::startReader(int devIndex){

    // local USBIO device instance, used to configure the device
    CUsbIo dev;
    // local instance of our Reader class, used to read from the pipe
    USBReader reader;
    // helper variables
    USBIO_SET_CONFIGURATION config;
    DWORD status;

    // open the device
    status = dev.Open(devIndex,devList,&usbIoID);
    if ( status != USBIO_ERR_SUCCESS ) {
        printf("Could not open device: %x",status);
        return;
    }

    // set up the configuration request
    ZeroMemory(&config,sizeof(config));

    config.ConfigurationIndex = CFG_INDEX;
    config.NbOfInterfaces = CFG_NUM_INTERFACES;
    config.InterfaceList[0].InterfaceIndex = CFG_INTERFACE;
    config.InterfaceList[0].AlternateSettingIndex = CFG_ALTSETTING;
    config.InterfaceList[0].MaximumTransferSize = CFG_MAX_TRANSFER;
    // configure the device
    printf("Configuring the device...\n");
    status = dev.SetConfiguration(&config);
    if ( status != USBIO_ERR_SUCCESS ) {
        printf("Could not configure device: %x",status);
        return;
    }

    status = reader.Bind(devIndex,ENDPOINT,devList,&usbIoID);
    if ( status != USBIO_ERR_SUCCESS ) {
        printf("Binding failed.\n");
        dev.UnconfigureDevice();
        return;
    }

    if ( !reader.AllocateBuffers(ENDPOINT_FIFO_SIZE, NUM_BUFFERS) ) {
        printf("Unable to allocate buffer pool.\n");
        dev.UnconfigureDevice();
        return;
    }
    // start the worker thread
    printf("Starting worker thread...\n");
    if ( !reader.StartThread() ) {
        printf("Unable to start worker thread.\n");
        dev.UnconfigureDevice();
        return;
    }
    printf("Worker thread is running.\n");

    sendVendorRequest(dev,0xb3);

    printf("Press any key to stop the worker thread.\n\n");
    _getch();

    reader.ShutdownThread();

    // close pipe, close device
    // Note: This is not absolute necessary because each
    // instance will be closed by its destructor.
    reader.Close();
    dev.UnconfigureDevice();
    dev.Close();
}

void USBInterface::sendVendorRequest(CUsbIo dev, UCHAR req){
    printf("Sending Vendor Request: %x\n", req);
    USBIO_CLASS_OR_VENDOR_REQUEST request;
    // set up the request, this is device-specific
    ZeroMemory(&request,sizeof(request));
    request.Flags = 0;
    request.Type = RequestTypeVendor;
    request.Recipient = RecipientDevice;
    request.RequestTypeReservedBits = 0;
    request.Request = req;
    request.Value = 4;
    request.Index = 0;
    // send request
    DWORD bufSize = 0;
    DWORD status = dev.ClassOrVendorOutRequest(
                NULL,       // no buffer
                bufSize,   // no data Stage
                &request    // USBIO_CLASS_OR_VENDOR_REQUEST structure
                );
    printf("Status: %d\n",status);
}

