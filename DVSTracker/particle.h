#ifndef PARTICLE_H
#define PARTICLE_H

class Particle
{
public:
    Particle(float posX = 0, float posY = 0, float uncert = 0, float w = 0, double ts = 0);
    void set(float posX, float posY, float uncert, float w, double ts);

    float x,y;
    float uncertainty;
    float weight;
    double timeStamp;
};

#endif // PARTICLE_H
