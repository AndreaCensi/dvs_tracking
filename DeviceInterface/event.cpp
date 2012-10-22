#include "event.h"

Event::Event()
{
    posX = -1;
    posY = -1;
    polarity = 0;
    timeStamp = 0;
    special = false;
    cluster = 0;
    assigned = false;
}

Event::~Event(){

}

bool Event::isSpecial(){
    return special;
}
