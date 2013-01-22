#include "usbreader.h"
#include "event.h"

#define DVS128_FRAME_LENGTH 4
#define NUM_PACKET_BUFS 20

USBReader::USBReader(int usbFifoSize)
{
    packetBuffer = new PacketBuffer(NUM_PACKET_BUFS,usbFifoSize/DVS128_FRAME_LENGTH);
    mileStone = 0;
}

USBReader::~USBReader(){
    delete packetBuffer;
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
        printf("Incorrect data size: %d bytes\n",numBytes);
        return;
    }

    //
    EventPacket *packet = packetBuffer->getNextWritable();
    packet->reset();    //reset before writing into it!

    //fill packet
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
                event.x = (rawAddr >> 1) & 0x7f;
                event.y = (rawAddr >> 8) & 0x7f;
                event.timeStamp = (mileStone + (data[i+2] & 0xff | ((data[i+3] & 0xff) << 8)))/1000000.0;
                event.type = 1 - rawAddr & 1;
            }
            packet->add(event); // add event to current event packet
        }
    }
}

RingBuffer<Event>* USBReader::getEventBuffer(){
    return eventBuffer;
}

PacketBuffer* USBReader::getPacketBuffer(){
    return packetBuffer;
}
