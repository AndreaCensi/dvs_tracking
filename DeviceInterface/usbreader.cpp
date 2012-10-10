#include "usbreader.h"
#include "event.h"

#define FRAME_LENGTH 4

USBReader::USBReader(void (*process)(Event event))
{
    processEvent = process;
    rBuf = new RingBuffer<unsigned char>(4096,4);
    mileStone = 0;
}

USBReader::~USBReader(){
    delete rBuf;
}

void USBReader::ProcessData(CUsbIoBuf* buf){
    if ( buf->Status==USBIO_ERR_SUCCESS ){
        const char *data = (const char*)buf->Buffer();
        int numBytes = buf->BytesTransferred;
        if(numBytes%4 != 0){
            printf("Incorrect data size...");
        }
        else{
            //sock.writeDatagram(data,numBytes,QHostAddress::LocalHost,8991);
            readDVS128Event(data,numBytes);
        }
    }
    else{
        // read operation completed with error
        fprintf(stderr,"Read error: %x\n",buf->Status);
    }
}

void USBReader::readDVS128Event(const char *data, int numBytes){
    for(int i = 0; i < numBytes; i+=4){
        //data = &data[i];
        if((data[i+3] & 0x80) == 0x80){
            mileStone += 0x4000L;
        }
        else if ((data[i+3] & 0x40) == 0x40){
            mileStone = 0;
        }
        else{
            Event event;
            unsigned int rawAddr  = (data[i+0] & 0xFF) | ((data[i+1] & 0xFF) << 8);
            if((rawAddr & (0x8000)) != 0){
                //specialEvent
                event.xAddr = -1;
                event.yAddr = -1;
                event.special = true;
            }
            else{
                event.polarity = /*1 -*/ rawAddr & 1;
                event.xAddr = (rawAddr >> 1) & 0x7f;
                event.yAddr = (rawAddr >> 8) & 0x7f;
                event.timeStamp = mileStone + (data[i+2] & 0xff | ((data[i+3] & 0xff) << 8));
            }
            processEvent(event);
        }
    }
}
