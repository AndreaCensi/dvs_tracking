#include "GLCamWidget.h"
#include <QPainter>
#include <QTimer>
#include <QColor>
#include <stdio.h>

#define DVS_RES 128
#define SCALE_F 4

GLCamWidget::GLCamWidget(RingBuffer<Event> *buffer,QWidget *parent) : QGLWidget(parent)
{
    eventBuffer = buffer;
    img = new QImage(DVS_RES,DVS_RES,QImage::Format_RGB32);
    weights = 0;

    setWindowTitle(tr("DVS128"));
    int size = SCALE_F*DVS_RES;
    resize(size,size);


    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(40);
}

GLCamWidget::~GLCamWidget(){
    delete img;
}

void GLCamWidget::initializeGL(){
    int size = SCALE_F*DVS_RES;
    glViewport(0, 0, size,  size);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, 1.0, -1.0f);
    glMatrixMode(GL_MODELVIEW);
}

void GLCamWidget::paintGL(){
    glImg = QGLWidget::convertToGLFormat(*img);
    glDrawPixels(glImg.width(),glImg.height(),GL_RGBA,GL_UNSIGNED_BYTE,glImg.bits());
}

void GLCamWidget::resizeGL(int w, int h){
    int side = qMin(w, h);
    glViewport((w - side) / 2, (h - side) / 2, side, side);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, 1.0, -1.0f);
    glMatrixMode(GL_MODELVIEW);
}

void GLCamWidget::updateImage(Event *e){
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

void GLCamWidget::updateImage(int from, int size){
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

void GLCamWidget::updateImage(int x, int y, int greyValue){
    x = 127-x;
    y = 127-y;

    QColor color = QColor(greyValue,greyValue,greyValue);
    QRgb *pixel = (QRgb*)img->scanLine(y);
    pixel = &pixel[x];
    *pixel = color.rgb();
}

void GLCamWidget::paintEvent(QPaintEvent *){
    QPainter painter(this);

    paintGL();

    //draw ellipse according to weight
    if(weights != 0){
        for(int i = 0; i < 4;i++){
            for(int j = 0; j < weights[i]->maxima->size(); j++){
                int x = (127 - weights[i]->maxima->get(j)->x)*SCALE_F;
                int y = (127 - weights[i]->maxima->get(j)->y)*SCALE_F;
                int w = weights[i]->maxima->get(j)->weight;

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

                color.setAlpha(150);

                painter.setPen(color);
                painter.setBrush(color);

                int r = w/100;
                QPoint center(x,y);
                painter.drawEllipse(center,r,r);
            }
        }
    }
    reset(); //reset image
}

void GLCamWidget::updateImage(int x, int y,int w, int i){
    x = 127-x;
    y = 127-y;

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

    QRgb *pixel = (QRgb*)img->scanLine(y);
    pixel = &pixel[x];
    *pixel = color.rgb();
}

void GLCamWidget::setWeightBuffers(FrequencyAccumulator **weightBuffers){
    weights = weightBuffers;
}

// reset image
void GLCamWidget::reset(){
    QColor color = Qt::black;
    for(int x = 0; x < 128; x++){
        for(int y = 0; y < 128; y++){
            QRgb *pixel = (QRgb*)img->scanLine(y);
            pixel = &pixel[x];
            *pixel = color.rgb();
        }
    }
}
