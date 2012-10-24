#include <QApplication>

#include "usbreader.h"
#include "usbinterface.h"
#include "eventprocessor.h"
#include "camwidget.h"

int main(int argc, char **argv){
    QApplication app(argc,argv);
    EventProcessor ep;
    USBInterface inf(&ep);
    CamWidget cw(ep.getImage(),ep.getClusters());
    cw.show();

    inf.startReading();
    return app.exec();
}
