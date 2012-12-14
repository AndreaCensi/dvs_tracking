#include "event.h"
#include "stdio.h"

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
