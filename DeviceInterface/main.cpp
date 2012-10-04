#include "usbreader.h"
#include "usbinterface.h"
#include "eventreader.h"

int main(int argc, char **argv){
    USBInterface inf;
    short deviceIndex = inf.queryDevice();
    printf("Device nb: %d\n",deviceIndex);
//    if(deviceIndex > -1){
//        inf.startReader(deviceIndex);
//        return 0;
//    }
//    else
//        return 1;
    return 0;
}
