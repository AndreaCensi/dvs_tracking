#include <QApplication>

#include "usbreader.h"
#include "usbinterface.h"
#include "eventprocessor.h"
#include "camwidget.h"

int main(int argc, char **argv){
    QApplication app(argc,argv);
    EventProcessor ep;
    USBInterface inf(&EventProcessor::processEvent);
    inf.startReading();
    printf("Press any key to stop the worker thread.\n\n");
    _getch();
    inf.stopReading();
    return app.exec();
}
