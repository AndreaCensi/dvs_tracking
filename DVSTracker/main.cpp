#include <QApplication>

#include "dvs128interface.h"
#include "tracker.h"
#include "camwidget.h"
#include "udpinterface.h"

#include "particlefilter.h"
#include "combinationanalyzer.h"

//int main(int argc, char **argv){
//    QApplication app(argc,argv);

//    //init frequencies
//    std::vector<int> frequencies;
//    frequencies.push_back(900);
//    frequencies.push_back(1070);
//    frequencies.push_back(1240);
//    frequencies.push_back(1410);

//    //tracking
//    UDPInterface udpIf;
//    RingBuffer<Event> *buf = udpIf.getEventBuffer();

//    //    DVS128Interface dvs;
//    //    RingBuffer<Event> *buf = dvs.getReaderInstance()->getEventBuffer();

//    CamWidget widget(buf);
//    Tracker t(buf,frequencies);
//    //widget.setWeightBuffers(t.weightBuffers);
//    //widget.setParticleFilters(t.particleFilters);
//    t.setWidget(&widget);

//    widget.show();
//    t.start();

//    //    dvs.startReading();

//    int ret = app.exec();
//    //    dvs.stopReading();
//    t.stop();
//    return ret;
//}

//CombinationAnalyzer test
int main(){
    const int size = 3;
    ParticleFilter* pfs[size];
    for(int i = 0; i < size; i++){
        pfs[i] = new ParticleFilter(3,1.0,16.0,16.0);
        for(int j = 0; j < pfs[i]->size();j++){
            pfs[i]->particles[j]->x = rand()%128;
            pfs[i]->particles[j]->y = rand()%128;
            pfs[i]->particles[j]->weight = rand()%100+1;
        }
    }

    pfs[0]->particles[0]->x = pfs[1]->get(1)->x+1;
    pfs[0]->particles[0]->y = pfs[1]->get(1)->y+1;

    for(int i = 0; i < size; i++){
        for(int j = 0; j < pfs[i]->size();j++){
            Particle **p = pfs[i]->getSortedParticles();
            printf("#filter: %d, #p: %d, x: %d, y: %d, w: %d\n",i,j,
                   (int)p[j]->x,(int)p[j]->y,p[j]->weight);
        }
        printf("\n");
    }
    printf("\n\n");

    CombinationAnalyzer ca(size,4.0,10);
    CombinationChoice choice(size);
    ca.analyze(choice,0,pfs);
    Combinations *c = ca.getHypotheses();

    printf("\nFound:\n");
    for(int i = 0; i < c->size();i++){
        for(int j = 0; j < c->get(i)->size();j++){
            printf("%d ",c->get(i)->get(j));
        }
        printf(", w: %f\n",c->get(i)->score);
    }

    for(int i = 0; i < size; i++){
        delete pfs[i];
    }

    return 0;
}

////quicksort test
//int main(){
//    int size = 10;
//    ParticleFilter pf(size,2.0f,16.0f,8.0f);
//    for(int i = 0; i < pf.size();i++){
//        int w = rand()%100;
//        pf.particles[i]->set(0,0,0,w,0.1);
//    }
//    printf("unsorted:\n");
//    for(int i = 0; i < pf.size();i++){
//        printf("%d ",pf.particles[i]->weight);
//    }

//    pf.quicksort(pf.particles,0,pf.size()-1);

//    printf("\nsorted:\n");

//    for(int i = 0; i < pf.size();i++){
//        printf("%d ",pf.particles[i]->weight);
//    }
//    printf("\n");

//    return 0;
//}

//int main(int argc, char **argv){
//    ParticleFilter pf(1,2.0f,16.0f,8.0f);
//    Particle p(5,5,4.0,500,0.1);

//    float ts = 0.1;

//    for(int i = 0; i < 10; i++){
//        pf.updateUncertainty(&p,ts);
//        printf("uncert: %f\n",p.uncertainty);
//        ts += 0.1;
//    }
//    return 0;
//}

//particle filter test
//int main(int argc, char **argv){
//    ParticleFilter pf(2,2.0f,20.0f,10.0f);

//    Particle c1(5,5,4.0,500,0.1);
//    pf.updateParticles(&c1);

//    //add a particle
//    Particle *p;
//    for(int i = 0; i < pf.size();i++){
//        p = &pf.particles[i];
//        printf("\n---\ni: %d\n---\nx: %f\ny: %f\nsigma: %f\nw: %f\nts: %f\n",i,p->x,p->y,p->uncertainty,p->weight,p->timeStamp);
//    }
//    printf("\n");

//    //print current state

//    Particle c2(100,100,2.0,700,1.1);
//    pf.updateParticles(&c2);

//    //print current state
//    for(int i = 0; i < pf.size();i++){
//        p = &pf.particles[i];
//                printf("\n---\ni: %d\n---\nx: %f\ny: %f\nsigma: %f\nw: %f\nts: %f\n",i,p->x,p->y,p->uncertainty,p->weight,p->timeStamp);
//    }
//    printf("\n");

//    return 0;
//}

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

//    Filter f(size,0.5);
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

//    int m_size = 32;
//    int f_size = 5;
//    Map<float> *map = new Map<float>(m_size,m_size);
//    Filter f(5,0.5);

//    for(int y = 0; y < f_size; y++){
//        for(int x = 0; x < f_size; x++){
//            printf("%f ",f.kernel[x+y*f_size]);
//        }
//        printf("\n");
//    }
//    printf("\n");

//    float value = -1000.0f;
//    for(int i = 0; i < m_size*m_size; i++){
//        value = value*-1;
//        map->set(i,value);
//    }

//    for(int y = 0; y < m_size; y++){
//        for(int x = 0; x < m_size; x++){
//            printf("%f ",map->get(x,y));
//        }
//        printf("\n");
//    }
//    printf("\n");

//    map = f.smoothen(map);

//    for(int y = 0; y < m_size; y++){
//        for(int x = 0; x < m_size; x++){
//            printf("%f ",map->get(x,y));
//        }
//        printf("\n");
//    }

//    delete map;

//    return app.exec();
//}
