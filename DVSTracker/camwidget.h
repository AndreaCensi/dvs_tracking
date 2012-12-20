#ifndef CAMWIDGET_H
#define CAMWIDGET_H

#include <QGLWidget>
#include <QWidget>
#include "event.h"
#include "ringbuffer.h"
#include "frequencyaccumulator.h"

class CamWidget : public QGLWidget
{
    Q_OBJECT

public:
    CamWidget(RingBuffer<Event> *buffer = 0,QWidget *parent = 0);
    ~CamWidget();
    void setWeightBuffers(FrequencyAccumulator **weightBuffers);

protected:
    void paintEvent(QPaintEvent *);
    void paintGL();
    void resizeGL(int w, int h);
    void initializeGL();

private:
    void reset();

    QImage *img, glImg;
    RingBuffer<Event> *eventBuffer;
    int bufSize;
    FrequencyAccumulator **weights;

public slots:
    void updateImage(Event *e);
    void updateImage(int from, int size);
    void updateImage(int x, int y, int greyValue);
    void updateImage(int x, int y,int w, int i);
};

#endif // CAMWIDGET_H
