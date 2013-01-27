#ifndef LOCALMAXIMUM_H
#define LOCALMAXIMUM_H

//! A container class to store a local weight maximum
/*!
    \author Jonas Strubel
*/
class LocalMaximum
{
public:
    LocalMaximum(int posX = 0, int posY = 0, float w = 0);
    void set(int posX, int posY, float w);
    int x;
    int y;
    float weight;
};

#endif // LOCALMAXIMUM_H
