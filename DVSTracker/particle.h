#ifndef PARTICLE_H
#define PARTICLE_H

#include <cfloat>

//! A container for a particle used in ParticleFilter
/*!
    \author Jonas Strubel
*/
class Particle
{
public:
    Particle(float posX = 0, float posY = 0, float uncert = FLT_MAX, float w = 0, double ts = 0);
    void set(float posX, float posY, float uncert, float w, double ts);
    void reset();

    float x,y;
    float uncertainty;
    float weight;
    double timeStamp;
};

#endif // PARTICLE_H
