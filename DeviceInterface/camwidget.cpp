#include "camwidget.h"
#include <QPainter>
#include <QTimer>
#include <QColor>
#include <stdio.h>

#define DVS_RES 128

CamWidget::CamWidget(std::vector<Cluster*> *c, int *hist, QWidget *parent) : QWidget(parent)
{
    img = new QImage(DVS_RES,DVS_RES,QImage::Format_RGB32);
    clusters = c;
    histogram = hist;

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(20);
    setWindowTitle(tr("DVS128"));
    resize(512,512);
    counter = 0;
}

CamWidget::~CamWidget(){
    delete img;
}

void CamWidget::updateImage(Event *e){
    int x = 127-e->posX;
    int y = 127-e->posY;
    int type = e->polarity;
    QColor color;
    if(type == 1)
        color = Qt::red;
    else
        color = Qt::blue;
    QRgb *pixel = (QRgb*)img->scanLine(y);
    pixel = &pixel[x];
    *pixel = color.rgb();
}

void CamWidget::paintEvent(QPaintEvent *){
    QPainter painter(this);
    QRect rect(0,0,512,512);
    painter.drawImage(rect,*img);
    painter.setPen(Qt::green);
    //draw circle around cluster
    if(clusters->size() > 0){
        for(unsigned int i = 0; i < clusters->size(); i++){
            if(!clusters->at(i)->candidate){
                int x = (127-clusters->at(i)->posX)*4;
                int y = (127-clusters->at(i)->posY)*4;
                int contX = clusters->at(i)->contourX*4;
                int contY = clusters->at(i)->contourY*4;

                painter.drawEllipse(QPoint(x,y),contX,contY);
            }
//            printf("x,y: %f %f                     \r",clusters->at(i)->posX,clusters->at(i)->posY);
            //            printf("#clusters: %d  \r",clusters->size());
        }

        if(!clusters->at(0)->candidate){
            int phase = clusters->at(0)->transitionHistory->phase;
            int period= clusters->at(0)->transitionHistory->period;

            printf("phase: %d period: %d                     \r",phase, period);
        }
    }



//    painter.setPen(Qt::yellow);
//    // draw histogram
//    for(int i = 0; i < 256; i++){
//        int x = i*2;
//        int startY = 508;
//        int endY = startY-histogram[i]/128;
//        painter.drawLine(x,startY,x,endY);
//    }

    painter.end();

    //reset image
    QColor color = Qt::black;
    for(int x = 0; x < 128; x++){
        for(int y = 0; y < 128; y++){
            QRgb *pixel = (QRgb*)img->scanLine(y);
            pixel = &pixel[x];
            *pixel = color.rgb();
        }
    }
}
