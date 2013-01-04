#ifndef PARTICLEFILTER_H
#define PARTICLEFILTER_H

#include "maxima.h"
#include "particle.h"

class ParticleFilter
{
public:
    ParticleFilter(int numParticles, float defaultSigma, float maxSigma, float maxVelocity);
    ~ParticleFilter();
    void update(Maxima *maxima, double ts);
    Particle* get(int i);
    Particle** getSortedParticles();
    Particle* getMaxWeightParticle();
    int size();

private:
    void updateParticles(Particle *c);
    void updateUncertainty(Particle *p, double ts);
    bool hasExpired(Particle *p);
    bool insideCovariance(Particle *p, Particle *c);
    void merge(Particle *p, Particle *c);
    void quicksort(Particle **p, int first, int last);

    Particle *pivot;    //quicksort pivot

    Particle **particles;
    Particle *currentMaxWeight;

    float sigma_0;
    float vMax;
    float maxUncertainty;
    int length;
};

#endif // PARTICLEFILTER_H
