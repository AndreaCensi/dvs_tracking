#include <QApplication>

#include "dvs128interface.h"
#include "tracker.h"
#include "camwidget.h"
#include "udpinterface.h"

int main(int argc, char **argv){
    QApplication app(argc,argv);
    Tracker t;
    //    UDPInterface udpIf(&t);
    DVS128Interface dvs(&t);
    dvs.startReading();
    t.start();
    return app.exec();
}
