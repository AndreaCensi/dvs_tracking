#include "particle.h"

//! Constructor
/*!
    \param posX x coordinate of particle in image space. May have subpixel precision.
    \param posY y coordinate of particle in image space. May have subpixel precision.
    \param uncert Uncertainty of particle. Increases with time, decreases with update/merging with consecutive measurements.
    \param w Weight of the particle.
    \param ts Timestamp of particles last update.
*/
Particle::Particle(float posX, float posY, float uncert, float w, double ts)
{
    set(posX,posY,uncert,w,ts);
}

void Particle::set(float posX, float posY, float uncert, float w, double ts){
    x = posX;
    y = posY;
    uncertainty = uncert;
    weight = w;
    timeStamp = ts;
}

void Particle::reset(){
    set(0,0,FLT_MAX,0,0);
}
