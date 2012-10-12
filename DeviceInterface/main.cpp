#include <QApplication>

#include "usbreader.h"
#include "usbinterface.h"
#include "eventprocessor.h"
#include "camwidget.h"

int main(int argc, char **argv){
    QApplication app(argc,argv);
    //EventProcessor ep;
    CamWidget cw;
    cw.show();
    USBInterface inf/*(&EventProcessor::processEvent)*/;

    QObject::connect(  inf.getReaderInstance()->getSignalWrapper(),SIGNAL(publishEvent(int,int,int)),&cw, SLOT(newEvent(int,int,int)),Qt::QueuedConnection );

    inf.startReading();

//    printf("Press any key to stop transfer.\n\n");
//    _getch();

//    //test to set LED on board
//    //const char buffer = 0x0;
//    inf.sendVendorRequest(0xb4);

//    printf("Press any key to stop the worker thread.\n\n");
//    _getch();
//    inf.stopReading();
    return app.exec();
    //return 0;
}
