#include <QApplication>

#include "dvs128interface.h"
#include "eventprocessor.h"
#include "camwidget.h"
#include "udpinterface.h"

int main(int argc, char **argv){
    QApplication app(argc,argv);
    EventProcessor ep;
    UDPInterface udpIf(&ep);
//    DVS128Interface dvs(&ep);
//    dvs.startReading();
    ep.start();
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
//    Event e[6];
//    e[0].posX = 50;
//    e[0].posY = 50;

//    e[1].posX = 51;
//    e[1].posY = 50;

//    e[2].posX = 49;
//    e[2].posY = 50;

//    e[3].posX = 50;
//    e[3].posY = 49;

//    e[4].posX = 50;
//    e[4].posY = 51;

//    e[5].posX = 50;
//    e[5].posY = 55;

//    for(int i = 0; i < 6; i++){
//        e[i].timeStamp = 10000+i*50;
//        e[i].polarity = 1;
//        c.addEvent(e[i]);
//    }
//    printf("x: %f, y: %f\n",c.posX,c.posY);
//    printf("cX: %f, cY: %f\n",c.contourX,c.contourY);
//}

//transition test
//int main(int argc, char **argv){
//    QApplication app(argc,argv);
//    Cluster c;

//    int t = 250;
//    for(int i = 0; i < 32; i++){
//        int numEvents = rand()%15 + 4;
//        for(int j = 0; j < 1; j++){
//            Event e;
//            e.timeStamp = t + 950 + rand()%100;
//            e.polarity = i&0x1;
//            c.addEvent(e);
//            c.updateTS(e.timeStamp);

//        }
//        t+=500;
//    }

//    c.convert();
//    for(int i = 0; i < 150; i++){
//        int numEvents = rand()%15 + 4;
//        for(int j = 0; j < 1; j++){
//            Event e;
//            e.timeStamp = t + 950 + rand()%100;
//            e.polarity = i&0x1;
//            c.addEvent(e);
//            c.updateTS(e.timeStamp);
//        }
//        t+=500;
//    }

//    //    for(int i = 0; i < 4; i++){
//    //        printf("%d ",c.transitionHistory->at(i)->timeStamp);
//    //    }
//    //    printf("\n");

//    printf("phase: %d\n", c.transitionHistory->phase);
//    printf("period: %d\n", c.transitionHistory->period);

//    EventProcessor ep;
//    Event e;
//    e.polarity = 0;
//    e.timeStamp = 745+400;
//    float cost1 = ep.temporalCost(&e, &c);
//    e.polarity = 1;
//    float cost2 = ep.temporalCost(&e, &c);

//    printf("temporal cost 1: %f\n",cost1);
//    printf("temporal cost 2: %f\n",cost2);
//    return app.exec();
//}

//ringbuffer test
//int main(int argc, char **argv){
//    RingBuffer<int> buf;
//    for(int i = 0; i < 10; i++){
//        buf.add(i);
//    }

//    for(int i = 0; i < 10; i++){
//        printf("%d ",buf.buffer[i]);
//    }
//    printf("\navailable: %d\n",buf.available());
//    int *x;
//    while( (x = buf.getNext()) != 0){
//        printf("%d ",*x);
//    }
//    printf("\navailable: %d\n",buf.available());
//    for(int i = 0; i < 5; i++){
//        buf.add(i+10);
//    }
//    printf("\navailable: %d\n",buf.available());
//    while( (x = buf.getNext()) != 0){
//        printf("%d ",*x);
//    }
//    printf("\navailable: %d\n",buf.available());
//}

// momentextractor test
//int main(int argc, char **argv){
//    Cluster c;
//    int counter = 0;

//    for(int i = 0; i < c.moments->size;i++)
//        printf("%d ",c.moments->at(i).m00);
//    printf("\n");
//    printf("counter: %d\n",counter);
//    for(int i = 0; i < 10000;i++){
//        Event e;
//        e.posX = 50;
//        e.posY = 50;
//        e.timeStamp = counter;
//        c.addEvent(e);

//        counter += 10;
//    }

//    for(int i = 0; i < c.moments->size;i++)
//        printf("%d ",c.moments->at(i).m00);
//    printf("\n");
//    printf("counter: %d\n\n",counter);
//    printf("posX: %f, posY: %f\n", c.posX,c.posY);

//    return 0;
//}
