#include "localmaximum.h"

LocalMaximum::LocalMaximum(int posX, int posY, int w)
{
    x = posX;
    y = posY;
    weight = w;
}

void LocalMaximum::set(int posX, int posY, int w){
    x = posX;
    y = posY;
    weight = w;
}
