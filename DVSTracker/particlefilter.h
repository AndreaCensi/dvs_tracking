#ifndef PARTICLEFILTER_H
#define PARTICLEFILTER_H

#include "maxima.h"
#include "particle.h"

class ParticleFilter
{
public:
    ParticleFilter(int numParticles, float defaultSigma, float maxSigma, float maxVelocity);
    ~ParticleFilter();
    void update(Maxima *maxima, int ts);

//private:
    void updateParticles(Particle *c);
    void updateUncertainty(Particle *p, double ts);
    bool hasExpired(Particle *p);
    bool insideCovariance(Particle *p, Particle *c);
    void merge(Particle *p, Particle *c);

    Particle *particles;
    int size;
    float sigma_0;
    float vMax;
    float maxUncertainty;
};

#endif // PARTICLEFILTER_H
