#include "usbreader.h"


#define FRAME_LENGTH 32

USBReader::USBReader(/*void (*process)(char*)*/)
{
    //processEvent = process;
}

USBReader::~USBReader(){

}

void USBReader::ProcessData(CUsbIoBuf* buf){
    if ( buf->Status==USBIO_ERR_SUCCESS ){
        printf((char*)buf->Buffer());
        //printf(".");
//        if(buf->Size > FRAME_LENGTH)
//            processEvent(buf->Buffer());
    }
    else{
        // read operation completed with error
        TCHAR strbuf[256];
//         _ftprintf(stderr,TEXT("ProcessData: %s\n"),CUsbIo::ErrorText(strbuf,sizeof(strbuf)/sizeof(TCHAR),buf->Status));
        fprintf(stderr,"Read error: %x\n",buf->Status);
    }
}
