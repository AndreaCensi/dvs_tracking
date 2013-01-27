#include "localmaximum.h"

//! Constructor
/*!
    \param posX x coordinate of the pixel.
    \param posY y coordinate of the pixel.
    \param w Weight of the local maximum.
*/
LocalMaximum::LocalMaximum(int posX, int posY, float w)
{
    set(posX,posY,w);
}

void LocalMaximum::set(int posX, int posY, float w){
    x = posX;
    y = posY;
    weight = w;
}
