#include "usbreader.h"
#include "event.h"

#define DVS128_FRAME_LENGTH 4

USBReader::USBReader(EventProcessorBase *ep)
{
    eventProcessor = ep;
    mileStone = 0;
}

USBReader::~USBReader(){
}

void USBReader::ProcessData(CUsbIoBuf* buf){
    if ( buf->Status==USBIO_ERR_SUCCESS ){
        const char *data = (const char*)buf->Buffer();
        int numBytes = buf->BytesTransferred;
        readDVS128Event(data,numBytes);
    }
    else{
        fprintf(stderr,"Read error: %x\n",buf->Status);
    }
}

void USBReader::readDVS128Event(const char *data, int numBytes){
    if(numBytes%DVS128_FRAME_LENGTH != 0){
        printf("Incorrect data size!");
    }
    else{
        for(int i = 0; i < numBytes; i+=DVS128_FRAME_LENGTH){
            if((data[i+3] & 0x80) == 0x80){
                mileStone += 0x4000L;
            }
            else if ((data[i+3] & 0x40) == 0x40){
                mileStone = 0;
            }
            else{
                Event event;
                unsigned int rawAddr  = (data[i+0] & 0xFF) | ((data[i+1] & 0xFF) << 8);
                if((rawAddr & (0x8000)) != 0){  //special event
                    event.special = true;
                }
                else{
                    event.polarity = 1 - rawAddr & 1;
                    event.posX = (rawAddr >> 1) & 0x7f;
                    event.posY = (rawAddr >> 8) & 0x7f;
                    event.timeStamp = mileStone + (data[i+2] & 0xff | ((data[i+3] & 0xff) << 8));
                }
                //processEvent
                eventProcessor->processEvent(event);
            }
        }
    }
}
