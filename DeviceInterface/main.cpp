#include <QApplication>

#include "dvs128interface.h"
#include "eventprocessor.h"
#include "camwidget.h"
#include "eventgenerator.h"

int main(int argc, char **argv){
    QApplication app(argc,argv);
    EventProcessor ep;
    DVS128Interface dvs(&ep);
    dvs.startReading();
    return app.exec();
}


//activity test
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

//moment test
//int main(int argc, char **argv){
//    Cluster c;
//    Event e[5];
//    e[0].posX = 50;
//    e[0].posY = 50;

//    e[1].posX = 51;
//    e[1].posY = 50;

//    e[2].posX = 49;
//    e[2].posY = 50;

//    e[3].posX = 52;
//    e[3].posY = 53;

//    e[4].posX = 48;
//    e[4].posY = 50;

//    for(int i = 0; i < 5; i++){
//        e[i].timeStamp = 10000+i*4;
//        e[i].polarity = 1;
//        c.addEvent(e[i]);
//    }
//    printf("x: %f, y: %f\n",c.posX,c.posY);
//    printf("cX: %f, cY: %f\n",c.contourX,c.contourY);
//}
