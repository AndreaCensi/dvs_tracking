#include "event.h"

Event::Event()
{
    xAddr = -1;
    yAddr = -1;
    polarity = 0;
    timeStamp = 0;
    special = false;
}

Event::~Event(){

}

bool Event::isSpecial(){
    return special;
}
