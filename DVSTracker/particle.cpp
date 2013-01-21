#include "particle.h"

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
