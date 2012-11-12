#ifndef CAMWIDGET_H
#define CAMWIDGET_H

#include <QWidget>
#include "event.h"
#include "cluster.h"

class CamWidget : public QWidget
{
    Q_OBJECT

public:
    CamWidget(std::vector<Cluster*> *c = 0, int *hist = 0, QWidget *parent = 0);
    ~CamWidget();
    void updateImage(Event *e);

protected:
    void paintEvent(QPaintEvent *);

private:
    QImage *img;
    std::vector<Cluster*>* clusters;
    int *histogram;
    int counter;
};

#endif // CAMWIDGET_H
