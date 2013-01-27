#ifndef PARTICLEFILTER_H
#define PARTICLEFILTER_H

#include "maxima.h"
#include "particle.h"
#include <vector>

//! A particle filter class
/*!
    Stores a number of partcles for each frequency. Is updated with the maxima of weighted intervals.
    \author Jonas Strubel
*/
class ParticleFilter
{
public:
    ParticleFilter(int numParticles, float defaultSigma, float maxSigma, float minimumMergeDistance, float maxVelocity);
    ~ParticleFilter();
    void update(Maxima *maxima, double ts);
    Particle* get(int i);
    void sortParticles();
    Particle* getMaxWeightParticle();
    Particle* getMinUncertaintyParticle();
    int size();

//private:
    void updateParticles(Particle *c);
    void updateUncertainty(Particle *p, double ts);
    bool hasExpired(Particle *p);
    bool insideCovariance(Particle *p, Particle *c);
    void merge(Particle *p, Particle *c);
    void quicksort(Particle **p, int first, int last);

    Particle *pivot;    //quicksort pivot

    Particle **particles;
    Particle *currentMaxWeight;
    Particle *currentMinUncertainty;

    float sigma_0;
    float vMax;
    float maxUncertainty;
    float minMergeDistance;
    int length;
};

#endif // PARTICLEFILTER_H
