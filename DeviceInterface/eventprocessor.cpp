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
    for(unsigned int i = 0; i < clusters.size(); i++)
        delete clusters[i];
    delete img;
}

void EventProcessor::processEvent(Event *e){
    //assignToCluster(e);

    //image update
    updateImage(e);

    //provisory cleanup
    delete e;
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

float EventProcessor::getBoltzmanWeight(Event *e, Cluster *c){
    return exp(-SHARPNESS*distance(e,c));
}


//TODO: on/off candidate clusters?
void EventProcessor::assignToCluster(Event *e){
    if(clusters.empty()){
        Cluster *c = new Cluster();
        c->addEvent(e);
        clusters.push_back(c);
    }
    else{
        // get Boltzman weights
        float sumBoltz = 0;
        float *weights = new float[clusters.size()];
        for(unsigned int i = 0; i < clusters.size(); i++){
            weights[i] = getBoltzmanWeight(e,clusters[i]);
            sumBoltz += weights[i];
        }
        //normalize
        int mostProbIndex = 0;
        for(unsigned int i = 0; i < clusters.size();i++){
            weights[i] = weights[i]/sumBoltz;
            if(weights[i] > weights[mostProbIndex]){
                mostProbIndex = i;
            }
        }
        if(weights[mostProbIndex] > ASSIGN_PROB){
            clusters[mostProbIndex]->addEvent(e);
        }
        else{
            Cluster  *c = new Cluster();
            c->addEvent(e);
            clusters.push_back(c);
        }
        delete [] weights;
    }
}

QImage* EventProcessor::getImage(){
    return img;
}
