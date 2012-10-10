#include <QApplication>

#include "usbreader.h"
#include "usbinterface.h"
#include "eventreader.h"
#include "camwidget.h"

int main(int argc, char **argv){
    QApplication app(argc,argv);
    USBInterface inf(&EventReader::processEvent);
    inf.startReading();
    printf("Press any key to stop the worker thread.\n\n");
    _getch();
    inf.stopReading();
    return app.exec();
}
