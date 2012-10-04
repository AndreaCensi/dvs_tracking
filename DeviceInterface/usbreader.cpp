#include "usbreader.h"


#define FRAME_LENGTH 32

USBReader::USBReader(/*void (*process)(char*)*/)
{
    //processEvent = process;
}

USBReader::~USBReader(){

}

void USBReader::ProcessData(CUsbIoBuf *Buf){
    if ( Buf->Status==USBIO_ERR_SUCCESS ){
        //printf((char*)Buf->Buffer());
        printf(".");
//        if(Buf->Size > FRAME_LENGTH)
//            processEvent(Buf->Buffer());
    }
    else{
        // read operation completed with error
        //TCHAR strbuf[256];
        //fprintf(stderr,TEXT("ProcessData: %s\n"),CUsbIo::ErrorText(strbuf,sizeof(strbuf)/sizeof(TCHAR),Buf->Status));
        printf("Read operation completed with an error...");
    }
}
