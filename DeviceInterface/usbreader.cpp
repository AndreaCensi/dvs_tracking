#include "usbreader.h"


#define FRAME_LENGTH 4

USBReader::USBReader(/*void (*process)(unsigned char*)*/)
{
    rBuf = new RingBuffer<unsigned char>(4096,4);
    mileStone = 0;
}

USBReader::~USBReader(){

}

void USBReader::ProcessData(CUsbIoBuf* buf){
    if ( buf->Status==USBIO_ERR_SUCCESS ){
        const char *data = (const char*)buf->Buffer();
        int numBytes = buf->BytesTransferred;

        //        printf("BytesTransferred: %d\n", numBytes);

        //        for(int i = 0; i < numBytes; i++){
        //            rBuf->add(data[i]);
        //        }

        sock.writeDatagram(data,numBytes,QHostAddress::LocalHost,8991);

        //        while(rBuf->newData()){
        //            processEvent(rBuf->data());
        //        }
    }
    else{
        // read operation completed with error
        fprintf(stderr,"Read error: %x\n",buf->Status);
    }
}

void USBReader::processEvent(const char *data){
    if(data[3] & 0x80 == 0x80){
        mileStone += 0x4000L;
    }
    else if ((data[3] & 0x40) == 0x40){
        mileStone = 0;
    }
    else{
        struct Event event;

        event.rawAddr  = (data[0] & 0xFF) | ((data[1] & 0xFF) << 8);
        if(event.rawAddr & (0x8000) != 0){
            //specialEvent
            event.xAddr = -1;
            event.yAddr = -1;
        }
        else{
            event.polarity = 1 - event.rawAddr & 1;
            event.xAddr = (event.rawAddr >> 1) & 0x7f;
            event.yAddr = (event.rawAddr >> 8) & 0x7f;
            event.timeStamp = mileStone + (data[2] & 0xff | ((data[3] & 0xff) << 8));
        }
    }
}
