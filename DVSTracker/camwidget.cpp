#include "camwidget.h"
#include <QPainter>
#include <QTimer>
#include <QColor>
#include <stdio.h>

#define DVS_RES 128
#define SCALE_F 4

CamWidget::CamWidget(QWidget *parent) : QWidget(parent)
{
    img = new QImage(DVS_RES,DVS_RES,QImage::Format_RGB32);

    weights = 0;
    particleFilters = 0;
    combinations = 0;
    maxWeightParticles = new Particle[4];

    //Image output
    outputImageIndex = 0;
    save = true;

    setWindowTitle(tr("DVS128"));
    int size = SCALE_F*DVS_RES;
    resize(size,size);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(40);
}

CamWidget::~CamWidget(){
    delete img;
    delete maxWeightParticles;
}

void CamWidget::updateImage(Event *e){
    int x = 127-e->x;
    int y = 127-e->y;
    int type = e->type;
    QColor color;
    if(type == 1)
        color = Qt::red;
    else
        color = Qt::blue;
    QRgb *pixel = (QRgb*)img->scanLine(y);
    pixel = &pixel[x];
    *pixel = color.rgb();
}

void CamWidget::updateImage(int x, int y, int greyValue){
    x = 127-x;
    y = 127-y;

    QColor color = QColor(greyValue,greyValue,greyValue);
    QRgb *pixel = (QRgb*)img->scanLine(y);
    pixel = &pixel[x];
    *pixel = color.rgb();
}

void CamWidget::paintEvent(QPaintEvent *){
    QPainter painter(this);
    QRect rect(0,0,512,512);
    painter.drawImage(rect,*img);

    //draw ellipse according to weight
    if(weights != 0){
        for(int i = 0; i < 4;i++){
            for(int j = 0; j < weights[i]->maxima->size(); j++){
                int x = (127 - weights[i]->maxima->get(j)->x)*SCALE_F;
                int y = (127 - weights[i]->maxima->get(j)->y)*SCALE_F;
                int w = weights[i]->maxima->get(j)->weight;

                QColor color = getColor(i);

                color.setAlpha(150);

                painter.setPen(color);
                painter.setBrush(color);

                int r = w/100;
                QPoint center(x,y);
                painter.drawEllipse(center,r,r);
            }
        }
    }

    //draw particles
    if(particleFilters != 0){
        for(int i = 0; i < 4;i++){
            ParticleFilter *pf = particleFilters[i];
            for(int j = 0; j < pf->size(); j++){
                if(pf->get(j)->timeStamp == 0 || pf->get(j)->x == 0)
                    continue;
                int x = int((127 - pf->get(j)->x)*SCALE_F);
                int y = int((127 - pf->get(j)->y)*SCALE_F);
                float sigma = pf->get(j)->uncertainty;

                QColor color = getColor(i);

                color.setAlpha(150);

                painter.setPen(color);
                painter.setBrush(color);

                int r = int(sigma*SCALE_F);
                QPoint center(x,y);
                painter.drawEllipse(center,r,r);
            }
        }
    }

    //        if(combinations != 0 && particleFilters != 0){
    //            for(int i = 0; i < 4;i++){
    //                CombinationChoice *c = combinations->get(i);
    //                for(int j = 0; j < c->size(); j++){
    //                    ParticleFilter *pf = particleFilters[j];
    //                    int x = int((127 - pf->get(c->get(j))->x)*SCALE_F);
    //                    int y = int((127 - pf->get(c->get(j))->y)*SCALE_F);
    //                    float sigma = pf->get(c->get(j))->uncertainty;

    //                    QColor color = getColor(j);

    //                    color.setAlpha(150);

    //                    painter.setPen(color);
    //                    painter.setBrush(color);

    //                    int r = int(sigma*SCALE_F);
    //                    QPoint center(x,y);
    //                    painter.drawEllipse(center,r,r);
    //                }
    //            }
    //        }

    // draw particle with maximum weight
    for(int i = 0; i < 4;i++){
        Particle *p = &maxWeightParticles[i];

        if(p->timeStamp == 0)
            continue;

        int x = int((127 - p->x)*SCALE_F);
        int y = int((127 - p->y)*SCALE_F);
        float sigma = p->uncertainty;

        QColor color = getColor(i);

        color.setAlpha(150);

        painter.setPen(color);
        painter.setBrush(color);

        int r = int(sigma*SCALE_F);
        QPoint center(x,y);
        painter.drawEllipse(center,r,r);
    }

//    if(save)
//        saveImage();

    reset(); //reset image
}

void CamWidget::updateImage(int x, int y,int w, int i){
    x = 127-x;
    y = 127-y;

    QColor color = getColor(i);

    QRgb *pixel = (QRgb*)img->scanLine(y);
    pixel = &pixel[x];
    *pixel = color.rgb();
}

void CamWidget::updateMaxWeightParticle(int i, Particle *p){
    if(p != 0)
        maxWeightParticles[i].set(p->x,p->y,p->uncertainty,p->weight,p->timeStamp);
    else
        maxWeightParticles[i].reset();
}

void CamWidget::setWeightBuffers(FrequencyAccumulator **weightBuffers){
    weights = weightBuffers;
}

void CamWidget::setParticleFilters(ParticleFilter **pfs){
    particleFilters = pfs;
}

void CamWidget::setCombinations(Combinations *c){
    combinations = c;
}

void CamWidget::stopSaving(){
    save = false;
}

QColor CamWidget::getColor(int i){
    QColor color;
    switch (i){
    case 0:
        color = Qt::red;
        break;
    case 1:
        color = Qt::blue;
        break;
    case 2:
        color = Qt::green;
        break;
    case 3:
        color = Qt::yellow;
        break;
    }
    return color;
}

void CamWidget::saveImage(){
    QString filename = "checkerboard_";
    QString path = "C:/Users/giselher/Documents/uzh/calibration_images";
    QString fileExtension = ".jpg";
    QString fullpath = path + "/" + filename + QString::number(outputImageIndex) + fileExtension;
    img->save(fullpath,"jpg",100);
    outputImageIndex++;
}

// reset image
void CamWidget::reset(){
    QColor color = Qt::black;
    for(int x = 0; x < 128; x++){
        for(int y = 0; y < 128; y++){
            QRgb *pixel = (QRgb*)img->scanLine(y);
            pixel = &pixel[x];
            *pixel = color.rgb();
        }
    }
}
