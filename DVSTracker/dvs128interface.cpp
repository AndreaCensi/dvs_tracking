#include "stdafx.h"
#include "dvs128interface.h"

// USB reader thread
#define NUM_BUFFERS 5

//Device config
#define  VID 0x152A //vendor ID
#define  PID 0x8400 //product ID
#define  ENDPOINT 0x86
#define	 ENDPOINT_FIFO_SIZE 4096
#define  CFG_INDEX 0
#define  CFG_NUM_INTERFACES 1
#define  CFG_INTERFACE 0
#define  CFG_ALTSETTING 0
#define	 CFG_MAX_TRANSFER 4096

//Vendor requests
#define START_READ 0xb3
#define STOP_READ 0xb4

GUID usbIoID = USBIO_IID;
HDEVINFO devList;

DVS128Interface::DVS128Interface(EventProcessorBase *ep){
    devList = NULL;
    devIndex = -1;
    reader = new USBReader(ep);
}

DVS128Interface::~DVS128Interface(){
    stopReading();
    delete reader;
}

void DVS128Interface::startReading(){
    CUsbIo dev;
    USB_DEVICE_DESCRIPTOR devDesc;
    DWORD status;
    bool found = false;

    devList = CUsbIo::CreateDeviceList(&usbIoID);
    if (devList == NULL){
        printf("Unable to build a device list!\n");
    }

    // Open and query usb devices to find the right one
    for (int i = 0; i < 127; i++){
        status = dev.Open(i,devList,&usbIoID);
        if ( status != USBIO_ERR_SUCCESS ) {
            if ( status != USBIO_ERR_NO_SUCH_DEVICE_INSTANCE ){
                fprintf(stdout,"UsbDev.Open returned with error 0x%08X\n",status);
            }
            break;
        }
        // Query device descriptor for comparison with PID and VID
        status = dev.GetDeviceDescriptor(&devDesc);
        if ( status == USBIO_ERR_SUCCESS ){
            found = true;
            if ( devDesc.iSerialNumber!=0 ){
                dev.Close();
                if (devDesc.idVendor == VID && devDesc.idProduct == PID){
                    fprintf(stdout,"Device found, starting reader.\n");
                    devIndex = i;
                    startReaderThread(devIndex);
                    break;
                } else {
                    fprintf(stdout,"Device not recognized\n");
                }
            } else {
                fprintf(stdout,"Querying device descriptor failed, status:0x%08X\n",status);
                dev.Close();
            }
        }

        if ( !found ) {
            fprintf(stdout,"There are no USB devices attached to the USBIO driver.\n");
        }
        else
            fprintf(stdout,"Device found...");
        CUsbIo::DestroyDeviceList(devList);
    }
}

void DVS128Interface::stopReading(){
    sendVendorRequest(STOP_READ);
    reader->ShutdownThread();
    reader->Close();
}

void DVS128Interface::startReaderThread(int devIndex){
    CUsbIo dev; //Device instance
    USBIO_SET_CONFIGURATION config; // Device config
    DWORD status;

    // open the device
    status = dev.Open(devIndex,devList,&usbIoID);
    if ( status != USBIO_ERR_SUCCESS ) {
        printf("Could not open device: %x\n",status);
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
    printf("Configuring...\n");
    status = dev.SetConfiguration(&config);
    if ( status != USBIO_ERR_SUCCESS ) {
        printf("Could not configure device: %x\n",status);
        return;
    }

    status = reader->Bind(devIndex,ENDPOINT,devList,&usbIoID);
    if ( status != USBIO_ERR_SUCCESS ) {
        printf("Binding failed.\n");
        dev.UnconfigureDevice();
        return;
    }

    if ( !reader->AllocateBuffers(ENDPOINT_FIFO_SIZE, NUM_BUFFERS) ) {
        printf("Unable to allocate buffer pool.\n");
        dev.UnconfigureDevice();
        return;
    }
    // start the reader thread
    printf("Starting reader thread...\n");
    if ( !reader->StartThread() ) {
        printf("Unable to start reader thread.\n");
        dev.UnconfigureDevice();
        return;
    }

    sendVendorRequest(START_READ);
}

void DVS128Interface::sendVendorRequest(UCHAR req, const char *buf, DWORD bufSize){
    CUsbIo dev;
    DWORD status;
    // open the device
    if(!dev.IsOpen()){
        status = dev.Open(devIndex,devList,&usbIoID);
        if ( status != USBIO_ERR_SUCCESS ) {
            printf("Could not open device: %x",status);
            return;
        }
    }
    printf("Sending Vendor Request: %x\n", req);
    USBIO_CLASS_OR_VENDOR_REQUEST request;
    ZeroMemory(&request,sizeof(request));
    request.Flags = 0;
    request.Type = RequestTypeVendor;
    request.Recipient = RecipientDevice;
    request.RequestTypeReservedBits = 0;
    request.Request = req;
    request.Value = 4;
    request.Index = 0;
    status = dev.ClassOrVendorOutRequest(
                buf,
                bufSize,
                &request
                );
    printf("Status: %x\n",status);
    dev.Close();
}

USBReader* DVS128Interface::getReaderInstance(){
    return reader;
}

