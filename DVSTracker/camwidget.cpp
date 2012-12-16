#include "camwidget.h"
#include <QPainter>
#include <QTimer>
#include <QColor>
#include <stdio.h>

#define DVS_RES 128

CamWidget::CamWidget(RingBuffer<Event> *buffer,QWidget *parent) : QWidget(parent)
{
    eventBuffer = buffer;
    img = new QImage(DVS_RES,DVS_RES,QImage::Format_RGB32);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(20);
    setWindowTitle(tr("DVS128"));
    resize(512,512);
}

CamWidget::~CamWidget(){
    delete img;
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

void CamWidget::updateImage(int from, int size){
    int index = from;
    for(int i = 0; i < size;i++){
        if (index == eventBuffer->size())
            index = 0;
        Event *e = eventBuffer->ref(index);
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

        index++;
    }
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
