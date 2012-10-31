#include "event.h"

Event::Event()
{
    posX = 0;
    posY = 0;
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
