#ifndef CAMWIDGET_H
#define CAMWIDGET_H

#include <QWidget>
#include "event.h"
#include "ringbuffer.h"
#include "frequencyaccumulator.h"
#include "particlefilter.h"

class CamWidget : public QWidget
{
    Q_OBJECT

public:
    CamWidget(RingBuffer<Event> *buffer = 0,QWidget *parent = 0);
    ~CamWidget();
    void setWeightBuffers(FrequencyAccumulator **weightBuffers);
    void setParticleFilters(ParticleFilter **pfs);

protected:
    void paintEvent(QPaintEvent *);

private:
    void reset();

    QImage *img;
    RingBuffer<Event> *eventBuffer;
    int bufSize;
    FrequencyAccumulator **weights;
    ParticleFilter **particleFilters;

public slots:
    void updateImage(Event *e);
    void updateImage(int from, int size);
    void updateImage(int x, int y, int greyValue);
    void updateImage(int x, int y,int w, int i);
};

#endif // CAMWIDGET_H
