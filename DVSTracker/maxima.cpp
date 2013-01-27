#include "maxima.h"
#include "math.h"

//! Constructor
/*!
    \param numberOfMaxima The number of local maxima to gather.
    \param minimumDistance The minimum Distance between local maxima.
*/
Maxima::Maxima(int numberOfMaxima, float minimumDistance)
{
    numMaxima = numberOfMaxima;
    maxima = new LocalMaximum[numMaxima];
    minSquaredDistance = minimumDistance*minimumDistance;
}

Maxima::~Maxima(){
    delete [] maxima;
}

void Maxima::update(int x, int y, float weight){
    if (weight == 0)
        return;
    int smallest = 0;
    for(int i = 0; i < numMaxima;i++){
        if(squaredDistance(&maxima[i],x,y) < minSquaredDistance && maxima[i].weight > weight){
            smallest = -1;
            break;
        }
        else if(squaredDistance(&maxima[i],x,y) < minSquaredDistance && maxima[i].weight < weight){
            maxima[i].set(x,y,weight);
            smallest = -1;
            break;
        }
        if(maxima[i].weight < maxima[smallest].weight)
            smallest = i;
    }
    if(smallest != -1 && maxima[smallest].weight < weight){
        maxima[smallest].set(x,y,weight);
    }
}

LocalMaximum* Maxima::get(int i){
    return &maxima[i];
}

float Maxima::squaredDistance(LocalMaximum *m, int x, int y){
    return (pow(float(m->x-x),2) + pow(float(m->y-y),2));
}

void Maxima::reset(){
    for(int i = 0; i < numMaxima; i++)
        maxima[i].set(0,0,0);
}

int Maxima::size(){
    return numMaxima;
}
