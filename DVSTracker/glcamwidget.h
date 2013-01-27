#ifndef GLCamWidget_H
#define GLCamWidget_H

#include <QGLWidget>
#include "event.h"
#include "ringbuffer.h"
#include "frequencyaccumulator.h"

//! Legacy widget class.
/*!
    Moves the CamWidget class to OpenGL. Not finished due to Window scaling issues.
    \author Jonas Strubel
*/
class GLCamWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLCamWidget(RingBuffer<Event> *buffer = 0,QWidget *parent = 0);
    ~GLCamWidget();
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

#endif // GLCamWidget_H
