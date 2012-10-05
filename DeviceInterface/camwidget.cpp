#include "camwidget.h"
#include <QPainter>
#include <QTimer>
#include <QColor>

CamWidget::CamWidget(QWidget *parent) : QWidget(parent)
{
    img = new QImage(128,128,QImage::Format_RGB32);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(20);
    setWindowTitle(tr("DVS128"));
    resize(512, 512);
}

void CamWidget::update(int x, int y, bool type){
    QColor color;
    if(type == false)
        color = Qt::black;
    else
        color = Qt::white;
    img->setPixel(x,y,color.value());
}

void CamWidget::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QRect rect(0,0,500,500);
    painter.scale(3.0,3.0);
    painter.drawImage(rect,*img);

    for(int x = 0; x < 128; x++){
        for(int y = 0; y < 128; y++){
            img->setPixel(x,y,Qt::gray);
        }
    }
}
