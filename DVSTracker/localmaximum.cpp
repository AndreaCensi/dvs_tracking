#include "localmaximum.h"

LocalMaximum::LocalMaximum(int posX, int posY, float w)
{
    x = posX;
    y = posY;
    weight = w;
}

void LocalMaximum::set(int posX, int posY, float w){
    x = posX;
    y = posY;
    weight = w;
}
