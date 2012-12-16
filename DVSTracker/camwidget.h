#ifndef CAMWIDGET_H
#define CAMWIDGET_H

#include <QWidget>
#include "event.h"
#include "ringbuffer.h"

class CamWidget : public QWidget
{
    Q_OBJECT

public:
    CamWidget(RingBuffer<Event> *buffer = 0,QWidget *parent = 0);
    ~CamWidget();

protected:
    void paintEvent(QPaintEvent *);

private:
    QImage *img;
    RingBuffer<Event> *eventBuffer;
    int bufSize;

public slots:
    void updateImage(Event *e);
    void updateImage(int from, int size);
    void updateImage(int x, int y, int greyValue);
};

#endif // CAMWIDGET_H
