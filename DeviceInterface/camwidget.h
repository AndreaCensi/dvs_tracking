#ifndef CAMWIDGET_H
#define CAMWIDGET_H

#include <QWidget>

class CamWidget : public QWidget
{
    Q_OBJECT

public:
    CamWidget(QWidget *parent = 0);
public slots:
    void update(int x, int y, bool type);
protected:
    void paintEvent(QPaintEvent *event);
private:
    QImage *img;
};

#endif // CAMWIDGET_H
