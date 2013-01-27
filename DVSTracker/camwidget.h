#ifndef CAMWIDGET_H
#define CAMWIDGET_H

#include <QWidget>
#include "event.h"
#include "ringbuffer.h"
#include "frequencyaccumulator.h"
#include "particlefilter.h"
#include "particle.h"
#include "combinations.h"


//! A Qt widget to display various tracking outputs
/*!
    This includes raw camera output, weighted period samples and tracked LED positions.
    \author Jonas Strubel
*/
class CamWidget : public QWidget
{
    Q_OBJECT

public:
    CamWidget(QWidget *parent = 0);
    ~CamWidget();
    void setWeightBuffers(FrequencyAccumulator **weightBuffers);
    void setParticleFilters(ParticleFilter **pfs);
    void setCombinations(Combinations *c);
    void stopSaving();

protected:
    void paintEvent(QPaintEvent *);

private:
    void reset();
    QColor getColor(int i);
    void saveImage();

    QImage *img;
    int bufSize;
    FrequencyAccumulator **weights;
    ParticleFilter **particleFilters;
    Combinations *combinations;
    Particle *maxWeightParticles;
    int outputImageIndex;
    bool save;

public slots:
    void updateImage(Event *e);
    void updateImage(int x, int y, int greyValue);
    void updateImage(int x, int y,int w, int i);
    void updateMaxWeightParticle(int i, Particle *p);
};

#endif // CAMWIDGET_H
