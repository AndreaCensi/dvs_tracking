#include <QApplication>

#include "dvs128interface.h"
#include "tracker.h"
#include "camwidget.h"
#include "udpinterface.h"

int main(int argc, char **argv){
    QApplication app(argc,argv);

    //init frequencies
    std::vector<int> frequencies;
    frequencies.push_back(900);
    frequencies.push_back(1070);
    frequencies.push_back(1240);
    frequencies.push_back(1410);

    //tracking
    UDPInterface udpIf;
    //    DVS128Interface dvs();
    CamWidget widget(udpIf.getEventBuffer());
    Tracker t(udpIf.getEventBuffer(),frequencies);
    t.setWidget(&widget);



    //connect signals/slots
    QObject::connect(&t,SIGNAL(updateCamWidget(int,int)),
                     &widget,SLOT(updateImage(int,int)),Qt::QueuedConnection);

    widget.show();
    t.start();
    //    dvs.startReading();

    int ret = app.exec();
    //    dvs.stopReading();
    t.stop();
    return ret;
}

//interval test
//int main(int argc, char **argv){
//    QApplication app(argc,argv);
//    Event e[5];

//    e[0].x = 15;
//    e[0].y = 37;
//    e[0].timeStamp = 0.123410;
//    e[0].type = 1;

//    e[1].x = 15;
//    e[1].y = 37;
//    e[1].timeStamp = 0.123745;
//    e[1].type = 0;

//    e[2].x = 15;
//    e[2].y = 37;
//    e[2].timeStamp = 0.124745;
//    e[2].type = 1;

//    e[3].x = 15;
//    e[3].y = 37;
//    e[3].timeStamp = 0.125745;
//    e[3].type = 0;

//    e[4].x = 15;
//    e[4].y = 37;
//    e[4].timeStamp = 0.126745;
//    e[4].type = 1;

//    //init frequencies
//    std::vector<int> frequencies;
//    frequencies.push_back(900);
//    frequencies.push_back(1070);
//    frequencies.push_back(1240);
//    frequencies.push_back(1410);

//    Tracker tracker(frequencies);

//    for(int i = 0; i < 4; i++){
//        tracker.processEvent(e[i]);
//    }

//    return app.exec();
//}


// test gaussian kernel
//int main(int argc, char **argv){
//    QApplication app(argc,argv);

//    int size = 5;

//    Filter f(size,1.0);
//    float sum = 0;
//    for(int y = 0; y < size; y++){
//        for(int x = 0; x < size; x++){
//            printf("%f ",f.kernel[x+y*size]);
//            sum += f.kernel[x+y*size];
//        }
//        printf("\n");
//    }
//    printf("sum: %f\n",sum);
//    return app.exec();
//}

//frequency accumulator test
//int main(int argc, char **argv){
//    QApplication app(argc,argv);
//    FrequencyAccumulator f(500,0.04,5,1.0,5.0,5,5);
//    int size = 5;
//    Interval iv[5];

//    iv[0].x = 3;
//    iv[0].y = 3;
//    iv[0].timeStamp = 0.001;
//    iv[0].deltaT  = 0.0022;

//    iv[1].x = 4;
//    iv[1].y = 3;
//    iv[1].timeStamp = 0.0011;
//    iv[1].deltaT  = 0.0017;

//    iv[2].x = 3;
//    iv[2].y = 4;
//    iv[2].timeStamp = 0.0012;
//    iv[2].deltaT  = 0.0015;

//    iv[3].x = 0;
//    iv[3].y = 0;
//    iv[3].timeStamp = 0.0016;
//    iv[3].deltaT  = 0.0020;

//    iv[4].x = 0;
//    iv[4].y = 0;
//    iv[4].timeStamp = 0.0028;
//    iv[4].deltaT  = 0.0020;

//    for(int i = 0; i < size; i++){
//        f.update(iv[i]);
//        if(f.hasExpired()){
//            //print map
//            for(int y = 0; y < size; y++){
//                for(int x = 0; x < size; x++){
//                    printf("%f ",f.weightMap->get(x,y));
//                }
//                printf("\n");
//            }
//            printf("\n");
//        }
//    }

//    return app.exec();
//}

// test filtering
//int main(int argc, char **argv){
//    QApplication app(argc,argv);

//    int size = 5;
//    Map<float> *map = new Map<float>(size,size);
//    Filter f(3,0.5);

//    for(int y = 0; y < 3; y++){
//        for(int x = 0; x < 3; x++){
//            printf("%f ",f.kernel[x+y*3]);
//        }
//        printf("\n");
//    }
//    printf("\n");

//    float value = -0.5f;
//    for(int i = 0; i < size*size; i++){
//        value = value*-1;
//        map->set(i,value);
//    }

//    for(int y = 0; y < size; y++){
//        for(int x = 0; x < size; x++){
//            printf("%f ",map->get(x,y));
//        }
//        printf("\n");
//    }
//    printf("\n");

//    map = f.smoothen(map);

//    for(int y = 0; y < size; y++){
//        for(int x = 0; x < size; x++){
//            printf("%f ",map->get(x,y));
//        }
//        printf("\n");
//    }

//    delete map;

//    return app.exec();
//}
