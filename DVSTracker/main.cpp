#include <QApplication>

#include "dvs128interface.h"
#include "tracker.h"
#include "camwidget.h"
#include "udpinterface.h"

// DVS IF
//int main(int argc, char **argv){
//    QApplication app(argc,argv);

//    //init frequencies
//    std::vector<int> frequencies;
//    //    frequencies.push_back(900);
//    //    frequencies.push_back(1070);
//    //    frequencies.push_back(1240);
//    //    frequencies.push_back(1410);

//    frequencies.push_back(740);
//    frequencies.push_back(1030);
//    frequencies.push_back(1320);
//    frequencies.push_back(1610);

//    //init camera and object data
//    std::vector<cv::Point3f> modelPts;
//    modelPts.push_back(cv::Point3f(0.1f,0.1f,0.0f));
//    modelPts.push_back(cv::Point3f(-0.1f,0.1f,0.0f));
//    modelPts.push_back(cv::Point3f(0.1f,-0.1f,0.0f));
//    modelPts.push_back(cv::Point3f(-0.1f,-0.1f,0.0f));


//    cv::Mat objectPoints(modelPts);
//    objectPoints = objectPoints.reshape(1);

//    cv::Mat cameraMatrix(3,3,CV_32F, cv::Scalar(0));
//    cameraMatrix.at<float>(0,0) = 100;
//    cameraMatrix.at<float>(1,1) = 100;
//    cameraMatrix.at<float>(2,2) = 1;
//    cameraMatrix.at<float>(0,2) = 64;
//    cameraMatrix.at<float>(1,2) = 64;

//    cv::Mat distortionCoeffs(5,1,CV_32F, cv::Scalar(0));


//    // EventPacket buffering
//    DVS128Interface dvs;
//    PacketBuffer *buf = dvs.getReaderInstance()->getPacketBuffer();

//    // Tracking
//    Tracker t(buf,frequencies,objectPoints,cameraMatrix,distortionCoeffs);

//    // Output widget
//    CamWidget widget;
//    t.setWidget(&widget);
//    //widget.setWeightBuffers(t.weightBuffers);
//    //    widget.setParticleFilters(t.particleFilters);

//    // Start the show
//    widget.show();
//    t.start();
//    dvs.startReading();
//    int ret = app.exec();
//    dvs.stopReading();
//    t.stop();
//    return ret;
//}

//UDP IF
int main(int argc, char **argv){
    QApplication app(argc,argv);

    //init frequencies
    std::vector<int> frequencies;

    //    frequencies.push_back(900);
    //    frequencies.push_back(1070);
    //    frequencies.push_back(1240);
    //    frequencies.push_back(1410);

    frequencies.push_back(740);
    frequencies.push_back(1030);
    frequencies.push_back(1320);
    frequencies.push_back(1610);

    //init camera and object data
    std::vector<cv::Point3f> modelPts;
    modelPts.push_back(cv::Point3f(0.1f,0.1f,0.0f));
    modelPts.push_back(cv::Point3f(-0.1f,0.1f,0.0f));
    modelPts.push_back(cv::Point3f(0.1f,-0.1f,0.0f));
    modelPts.push_back(cv::Point3f(-0.1f,-0.1f,0.0f));
    cv::Mat objectPoints(modelPts);
    objectPoints = objectPoints.reshape(1);

    cv::Mat cameraMatrix(3,3,CV_32F, cv::Scalar(0));
    cameraMatrix.at<float>(0,0) = 100;
    cameraMatrix.at<float>(1,1) = 100;
    cameraMatrix.at<float>(2,2) = 1;
    cameraMatrix.at<float>(0,2) = 64;
    cameraMatrix.at<float>(1,2) = 64;

    cv::Mat distortionCoeffs(5,1,CV_32F, cv::Scalar(0));

    // EventPacket buffering
    UDPInterface udpIf;
    PacketBuffer *buf = udpIf.getPacketBuffer();

    // Tracking
    Tracker t(buf,frequencies,objectPoints,cameraMatrix,distortionCoeffs);

    // Output widget
    CamWidget widget;
    t.setWidget(&widget);

    // Start the show
    widget.show();
    t.start();
    int ret = app.exec();
    t.stop();
    return ret;
}

//Rodrigues test
//int main(){
//    cv::Mat in(3,1,CV_32F);
//    in.at<float>(0,0) = 1;
//    in.at<float>(1,0) = 2;
//    in.at<float>(2,0) = 3;

//    cv::Mat out;
//    cv::Rodrigues(in,out);

//    for(int i = 0; i < out.rows;i++){
//        for(int j = 0; j < out.cols;j++){
//            printf("%f ",out.at<float>(i,j));
//        }
//        printf("\n");
//    }


//    return 0;
//}

//solvepnp test
//int main(){
//    const float PI = 3.14159265f;

//    //init camera and object data
//    std::vector<cv::Point3f> modelPts;
//    modelPts.push_back(cv::Point3f(-0.1f,-0.1f,0.0f));
//    modelPts.push_back(cv::Point3f(-0.1f,0.1f,0.0f));
//    modelPts.push_back(cv::Point3f(0.1f,-0.1f,0.0f));
//    modelPts.push_back(cv::Point3f(0.1f,0.1f,0.0f));
//    cv::Mat objectPoints(modelPts);
//    objectPoints = objectPoints.reshape(1);

//    cv::Mat cameraMatrix(3,3,CV_32F, cv::Scalar(0));
//    cameraMatrix.at<float>(0,0) = 100;
//    cameraMatrix.at<float>(1,1) = 100;
//    cameraMatrix.at<float>(2,2) = 1;
//    cameraMatrix.at<float>(0,2) = 64;
//    cameraMatrix.at<float>(1,2) = 64;

//    cv::Mat distortionCoeffs(5,1,CV_32F, cv::Scalar(0));

//    PoseEstimation pe(objectPoints,cameraMatrix,distortionCoeffs);

//    cv::Mat imagePoints(4,2,CV_32F);
//    int m = 63;
//    imagePoints.at<float>(0,0) = m-10;
//    imagePoints.at<float>(0,1) = m-10;

//    imagePoints.at<float>(1,0) = m-10;
//    imagePoints.at<float>(1,1) = m+10;

//    imagePoints.at<float>(2,0) = m+10;
//    imagePoints.at<float>(2,1) = m-10;

//    imagePoints.at<float>(3,0) = m+10;
//    imagePoints.at<float>(3,1) = m+10;

//    pe.estimatePose(imagePoints);
//    cv::Mat rvec = pe.getRotationVector();
//    cv::Mat tvec = pe.getTranslationVector();

//    double x,y,z,rx,ry,rz;

//    x = tvec.at<double>(0,0);
//    y = tvec.at<double>(1,0);
//    z = tvec.at<double>(2,0);

//    rx = rvec.at<double>(0,0);
//    ry = rvec.at<double>(1,0);
//    rz = rvec.at<double>(2,0);

//    printf("[x y z (cm)]: %3.0f %3.0f %3.0f\t [Y P R (deg)]: %3.1f %3.1f %3.1f     \n",x*100,y*100,z*100,rx*180/PI,ry*180/PI,rz*180/PI);

//    imagePoints.at<float>(0,0) = m+10;
//    imagePoints.at<float>(0,1) = m+10;

//    imagePoints.at<float>(1,0) = m-10;
//    imagePoints.at<float>(1,1) = m-10;

//    imagePoints.at<float>(2,0) = m-10;
//    imagePoints.at<float>(2,1) = m+10;

//    imagePoints.at<float>(3,0) = m+10;
//    imagePoints.at<float>(3,1) = m-10;

//    pe.estimatePose(imagePoints);
//    rvec = pe.getRotationVector();
//    tvec = pe.getTranslationVector();

//    x = tvec.at<double>(0,0);
//    y = tvec.at<double>(1,0);
//    z = tvec.at<double>(2,0);

//    rx = rvec.at<double>(0,0);
//    ry = rvec.at<double>(1,0);
//    rz = rvec.at<double>(2,0);

//    printf("[x y z (cm)]: %3.0f %3.0f %3.0f\t [Y P R (deg)]: %3.1f %3.1f %3.1f     \n",x*100,y*100,z*100,rx*180/PI,ry*180/PI,rz*180/PI);

//    return 0;
//}

////matrix test
//void printMatrix(cv::Mat m){
//    int w = m.cols;
//    int h = m.rows;
//    for(int i = 0; i < h; i++){
//        for(int j = 0; j < w; j++){
//            printf("%f ",m.at<float>(i,j));
//        }
//        printf("\n");
//    }
//    printf("rows: %d, colums: %d, channels: %d\n\n",m.rows,m.cols,m.channels());
//}

//int main(){
//    //init camera and object data
//    std::vector<cv::Point3f> modelPts;
//    modelPts.push_back(cv::Point3f(0,0,0));
//    modelPts.push_back(cv::Point3f(0,20,0));
//    modelPts.push_back(cv::Point3f(20,0,0));
//    modelPts.push_back(cv::Point3f(20,20,0));
//    cv::Mat objectPoints(modelPts);
//    objectPoints = objectPoints.reshape(1);

//    cv::Mat cameraMatrix(3,3,CV_32F, cv::Scalar(0));
//    cameraMatrix.at<float>(0,0) = 64;
//    cameraMatrix.at<float>(1,1) = 64;
//    cameraMatrix.at<float>(2,2) = 1;
//    cameraMatrix.at<float>(0,2) = 32;
//    cameraMatrix.at<float>(1,2) = 32;

//    cv::Mat imagePoints(4,2,CV_32F);
//    for( int i = 0; i < 4;i++){
//        imagePoints.at<float>(i,0) = i;
//        imagePoints.at<float>(i,1) = i;
//    }

//    printMatrix(objectPoints);
//    printMatrix(cameraMatrix);
//    printMatrix(imagePoints);

//    int npoints = std::max(objectPoints.checkVector(3, CV_32F), objectPoints.checkVector(3, CV_64F));
//    CV_Assert( npoints >= 0 && npoints == std::max(imagePoints.checkVector(2, CV_32F), imagePoints.checkVector(2, CV_64F)) );

//    return 0;
//}

////pose estimation test
//int main(){
//    cv::Mat m;
//    PoseEstimation pe(m,m,m);
//    pe.test();
//    return 0;
//}

////max weight test
//int main(int argc, char **argv){
//    float sd = 0.0005f;
//    float fr = 500.0f;
//    FrequencyAccumulator f(fr,sd,5,1.0,5.0,5,5);
//    int weight = f.getWeight(1/fr,fr,sd);
//    printf("weight: %d\n",weight);
//    return 0;
//}

////CombinationAnalyzer test
//int main(){
//    const int size = 4; //number of tracks
//    ParticleFilter* pfs[size];
//    srand(0);
//    for(int i = 0; i < size; i++){
//        pfs[i] = new ParticleFilter(8,1.0,16.0,1.0,200.0);
//        for(int j = 0; j < pfs[i]->size();j++){
//            pfs[i]->particles[j]->x = rand()%128;
//            pfs[i]->particles[j]->y = rand()%128;
//            pfs[i]->particles[j]->weight = rand()%100+1;
//        }
//    }

//    for(int i = 0; i < size; i++){
//        pfs[i]->sortParticles();
//    }

//    pfs[3]->particles[2]->x = pfs[1]->get(0)->x+1;
//    pfs[3]->particles[2]->y = pfs[1]->get(0)->y+1;

//    for(int i = 0; i < size; i++){
//        for(int j = 0; j < pfs[i]->size();j++){
//            Particle **p = pfs[i]->particles;

//            printf("#[filter particle]: %d %d, x: %d, y: %d, w: %d\n",i,j,
//                   (int)p[j]->x,(int)p[j]->y,p[j]->weight);
//        }
//        printf("\n");
//    }
//    printf("\n");

//    CombinationAnalyzer ca(pfs,size,8.0,20);
//    ca.evaluate();
//    Combinations *c = ca.getHypotheses();

//    printf("Found:\n");
//    for(int i = 0; i < c->size();i++){
//        for(int j = 0; j < c->get(i)->size();j++){
//            printf("%d ",c->get(i)->get(j));
//        }
//        printf(", u: %f\n",c->get(i)->score);
//    }

//    printf("\n#Evals: %d\n",ca.counter);

//    for(int i = 0; i < size; i++){
//        delete pfs[i];
//    }

//    return 0;
//}

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
//                    printf("%d ",f.weightMap->get(x,y));
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
