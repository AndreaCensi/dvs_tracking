#include "usbreader.h"


#define FRAME_LENGTH 4

USBReader::USBReader(/*void (*process)(unsigned char*)*/)
{
    rBuf = new RingBuffer<unsigned char>(4096,4);
    mileStone = 0;
    camWidget = new CamWidget();
    camWidget->show();
}

USBReader::~USBReader(){
    delete camWidget;
}

void USBReader::ProcessData(CUsbIoBuf* buf){
    if ( buf->Status==USBIO_ERR_SUCCESS ){
        const char *data = (const char*)buf->Buffer();
        for(int i = 0; i < buf->Size(); i++){
            rBuf->add(data[i]);
        }

        //printf("bufSize: %d\n",buf->Size());
        printf(".");
//        QUdpSocket sock;
//        int s = sock.writeDatagram(data,buf->Size(),QHostAddress::LocalHost,8991);
        //printf("%d",s);
        //        while(rBuf->newData()){
        //            processEvent(rBuf->data());
        //        }
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
        camWidget->update(event.xAddr,event.yAddr,event.polarity);
    }



    //    QByteArray bytes;
    //    for(int i = 0; i < 4;i++)
    //        bytes.append(data[i]);
    //    udpClient.send(bytes,"localhost",8991);
}
