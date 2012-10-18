#include "eventprocessor.h"
#include "stdio.h"
#include <math.h>

//parameters from thesis
#define SHARPNESS 0.7
#define ASSIGN_PROB 0.6

EventProcessor::EventProcessor(){
    img = new QImage(128,128,QImage::Format_RGB32);
}

EventProcessor::~EventProcessor(){
    delete img;
}

void EventProcessor::processEvent(Event *e){
    assignToCluster(e);

    //image update
    updateImage(e);

    //provisory cleanup
    delete e;
}

QImage* EventProcessor::getImage(){
    return img;
}

void EventProcessor::updateImage(Event *e){
    int x = e->posX;
    int y = e->posY;
    int type = e->polarity;
    QColor color;
    if(type == 1)
        color = Qt::red;
    else
        color = Qt::blue;
    QRgb *pixel = (QRgb*)img->scanLine(127-y);
    pixel = &pixel[127-x];
    *pixel = color.rgb();
}

//distance with circular boundary
float EventProcessor::distance(Event *e, Cluster *c){
    return sqrt(pow(float(e->posX-c->posX),2) + pow(float(e->posY-c->posY),2));
}

float EventProcessor::distance(Event *e1, Event *e2){
    return sqrt(pow(float(e1->posX-e2->posX),2) + pow(float(e1->posY-e2->posY),2));
}

float EventProcessor::getBoltzmanWeight(Event *e, Cluster *c){
    return exp(-SHARPNESS*distance(e,c));
}


void EventProcessor::assignToCluster(Event *e){
    //..
}
