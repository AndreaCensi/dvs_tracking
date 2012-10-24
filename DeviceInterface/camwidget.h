#ifndef CAMWIDGET_H
#define CAMWIDGET_H

#include <QWidget>
#include "event.h"
#include "cluster.h"

class CamWidget : public QWidget
{
    Q_OBJECT

public:
    CamWidget(QImage *i, std::vector<Cluster*> *c, QWidget *parent = 0);
    void setImage(QImage *i);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QImage *img;
    std::vector<Cluster*>* clusters;
    int counter;
};

#endif // CAMWIDGET_H
