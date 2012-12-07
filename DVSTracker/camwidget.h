#ifndef CAMWIDGET_H
#define CAMWIDGET_H

#include <QWidget>
#include "event.h"

class CamWidget : public QWidget
{
    Q_OBJECT

public:
    CamWidget(QWidget *parent = 0);
    ~CamWidget();
    void updateImage(Event *e);

protected:
    void paintEvent(QPaintEvent *);

private:
    QImage *img;
};

#endif // CAMWIDGET_H
