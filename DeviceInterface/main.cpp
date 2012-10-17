#include <QApplication>

#include "usbreader.h"
#include "usbinterface.h"
#include "eventprocessor.h"
#include "camwidget.h"

int main(int argc, char **argv){
    QApplication app(argc,argv);
    EventProcessor ep;
    USBInterface inf(&ep);
    CamWidget cw;
    cw.show();

    QObject::connect( ep.getSignalWrapper(),SIGNAL(publishEvent(int,int,int)),&cw, SLOT(newEvent(int,int,int)));

    inf.startReading();

//    printf("Press any key to stop transfer.\n\n");
//    _getch();

//    printf("Press any key to stop the worker thread.\n\n");
//    _getch();
//    inf.stopReading();
    return app.exec();
    //return 0;
}
