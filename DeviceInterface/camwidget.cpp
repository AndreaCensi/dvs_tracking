#include "camwidget.h"
#include <QPainter>
#include <QTimer>
#include <QColor>
#include <stdio.h>

CamWidget::CamWidget(QWidget *parent) : QWidget(parent)
{
    img = new QImage(128,128,QImage::Format_RGB32);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(20);
    setWindowTitle(tr("DVS128"));
    resize(512,512);
    counter = 0;
}

void CamWidget::newEvent(int x, int y, int type){
    QColor color;
    if(type == 1)
        color = Qt::red;
    else
        color = Qt::blue;
    QRgb *pixel = (QRgb*)img->scanLine(127-y);
    pixel = &pixel[127-x];
    *pixel = color.rgb();
}

void CamWidget::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QRect rect(0,0,512,512);
    QColor color = Qt::black;
    painter.drawImage(rect,*img);
    painter.setPen(Qt::green);
    painter.drawEllipse(63*4,63*4,20,20);

    for(int x = 0; x < 128; x++){
        for(int y = 0; y < 128; y++){
            QRgb *pixel = (QRgb*)img->scanLine(y);
            pixel = &pixel[x];
            *pixel = color.rgb();
        }
    }
}


