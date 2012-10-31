#include <QApplication>

#include "dvs128interface.h"
#include "eventprocessor.h"
#include "camwidget.h"
#include "eventgenerator.h"

int main(int argc, char **argv){
    QApplication app(argc,argv);
    EventProcessor ep;
    DVS128Interface dvs(&ep);

    CamWidget cw(ep.getImage(),ep.getClusters());
    cw.show();

    dvs.startReading();
    return app.exec();
}

//int main(int argc, char **argv){
//    Cluster c;
//    int counter = 0;
//    for(int i = 0; i < 15;i++){
//        Event e;
//        e.timeStamp = counter;
//        c.addEvent(e);

//        for(int i = 0; i < c.eventsPerInterval->size;i++)
//            printf("%d ",c.eventsPerInterval->at(i));
//        printf("\ncounter: %d\n",counter);

//        counter += 1000;
//    }

//    counter += 5000;
//    c.updateTS(counter);

//    for(int i = 0; i < c.eventsPerInterval->size;i++)
//        printf("%d ",c.eventsPerInterval->at(i));
//    printf("\n");
//    printf("counter: %d\n",counter);

//    printf("activity: %f\n",c.getActivity());
//    return 0;
//}
