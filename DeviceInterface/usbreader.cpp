#include "usbreader.h"


#define FRAME_LENGTH 4

USBReader::USBReader(/*void (*process)(unsigned char*)*/)
{
    rBuf = new RingBuffer<unsigned char>(4096,4);
    processEvent = process;
}

USBReader::~USBReader(){

}

void USBReader::ProcessData(CUsbIoBuf* buf){
    if ( buf->Status==USBIO_ERR_SUCCESS ){
        unsigned char *data = (unsigned char*)buf->Buffer();
        for(int i = 0; i < buf->Size(); i++){
            rBuf->add(data[i]);
        }
        printf("bufSize: %d\n",buf->Size());
        while(rBuf->newData()){
            processEvent(rBuf->data());
        }
    }
    else{
        // read operation completed with error
        fprintf(stderr,"Read error: %x\n",buf->Status);
    }
}

void USBReader::processEvent(unsigned char *data){
    if(data[3] & 0x80 == 0x80){
        mileStone += 0x4000L;
    }
    else if ((data[3] & 0x40) == 0x40){
        mileStone = 0;
    }
    else{
        struct Event event;
        event.xAddr = 0x00;
        event.yAddr = 0x00;
        event.timeStamp = mileStone + (data[2] & 0xff | ((data[3] & 0xff) << 8));
        event.eventType = 0x1;
    }
}
