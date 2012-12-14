#include "filter.h"
#include <math.h>

#define OOB_AVG 0.5f        //avg value???-----------------------

const float Filter::PI = 3.14159265f;

Filter::Filter(int size, float standardDeviation)
{
    kernelSize = size;
    sd = standardDeviation;
    kernel = new float[size*size];
    generateKernel();
}

Filter::~Filter(){
    delete [] kernel;
}

Map<float>* Filter::smoothen(Map<float> *buffer){
    Map<float> *filteredMap = new Map<float>(buffer->height,buffer->width);

    int r = kernelSize/2;
    // Go through 2D map
    for(int h = 0; h < buffer->height; h++){
        for(int w = 0; w < buffer->width; w++){
            float filteredWeight = 0.0f;

            // Convolute with kernel
            for(int y = 0; y < kernelSize; y++){
                for(int x = 0; x < kernelSize; x++){
                    int u = w-r+x;
                    int v = h-r+y;
                    if(outOfBounds(buffer,u,v))
                        filteredWeight += (kernelGet(x,y) * OOB_AVG);
                    else
                        filteredWeight += (kernelGet(x,y) * buffer->get(u,v));
                }
            }
            // Set new value
            filteredMap->insert(w,h,filteredWeight);
        }
    }
    delete buffer;
    return filteredMap;
}

void Filter::generateKernel(){
    int r = kernelSize/2;
    float sum = 0;
    for(int y = -r; y <= r;y++){
        for(int x = -r; x <= r;x++){
            float value = (1.0f/(sd*sd*2*PI))*exp(-(x*x+y*y)/(2*pow(sd,2)));
            sum += value;
            kernelSet(x+r,y+r,value);
        }
    }

    //normalize kernel
    for(int i = 0; i < kernelSize*kernelSize;i++){
        kernel[i] = kernel[i]/sum;
    }
}

bool Filter::outOfBounds(Map<float> *buffer,int x, int y){
    if( x >= 0 && y >= 0 && x < buffer->width && y < buffer->height)
        return false;
    else
        return true;
}

float Filter::kernelGet(int x, int y){
    int index = x + y*kernelSize;
    return kernel[index];
}

void Filter::kernelSet(int x, int y, float value){
    int index = x + y*kernelSize;
    kernel[index] = value;
}
