#include <QApplication>

#include "usbreader.h"
#include "usbinterface.h"
#include "eventreader.h"
#include "camwidget.h"

int main(int argc, char **argv){
    QApplication app(argc,argv);
    USBInterface inf;
    inf.queryDevice();
    return app.exec();
}
