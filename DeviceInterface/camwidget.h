#ifndef CAMWIDGET_H
#define CAMWIDGET_H

#include <QWidget>
#include "event.h"

class CamWidget : public QWidget
{
    Q_OBJECT

public:
    CamWidget(QWidget *parent = 0);
public slots:
    void newEvent(int x, int y, int type);
protected:
    void paintEvent(QPaintEvent *event);
private:
    QImage *img;
    int counter;
};

#endif // CAMWIDGET_H
