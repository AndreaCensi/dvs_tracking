#include "event.h"
#include "stdio.h"

//! Constructor
/*!
    \param ts Timestamp of event.
    \param posX x coordinate of pixel.
    \param posY y coordinate of pixel.
    \param polarityType Type of change (ON/OFF).
    \param specialEvent Denotes if special event.
*/
Event::Event(double ts, int posX, int posY, int polarityType, bool specialEvent)
{
    timeStamp = ts;
    x = posX;
    y = posY;
    type = polarityType;
    special = specialEvent;
}

Event::~Event(){

}

bool Event::isSpecial(){
    return special;
}
