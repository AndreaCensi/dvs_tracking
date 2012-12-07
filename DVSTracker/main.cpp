#include <QApplication>

#include "dvs128interface.h"
#include "eventprocessor.h"
#include "camwidget.h"
#include "udpinterface.h"

int main(int argc, char **argv){
    QApplication app(argc,argv);
    EventProcessor ep;
    //    UDPInterface udpIf(&ep);
    DVS128Interface dvs(&ep);
    dvs.startReading();
    ep.start();
    return app.exec();
}
