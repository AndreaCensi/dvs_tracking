#include "event.h"

Event::Event(unsigned int ts, int posX, int posY, int polarityType, int specialEvent)
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
