#ifndef CAMWIDGET_H
#define CAMWIDGET_H

#include <QWidget>
#include "event.h"

class CamWidget : public QWidget
{
    Q_OBJECT

public:
    CamWidget(QImage *i, QWidget *parent = 0);
    void setImage(QImage *i);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QImage *img;
    int counter;
};

#endif // CAMWIDGET_H
