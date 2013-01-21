#include "filter.h"
#include <math.h>

const float Filter::PI = 3.14159265f;

static int int3x3kernel[9] = {1,2,1,2,4,2,1,2,1};

Filter::Filter(int size, float standardDeviation, int mapW, int mapH)
{
    filteredMap = new Map<float>(mapW,mapH);
    kernelSize = size;
    sd = standardDeviation;
    kernel = new float[size*size];
    generateKernel();
}

Filter::~Filter(){
    delete filteredMap;
    delete [] kernel;
}

Map<float>* Filter::smoothen(Map<float> *buffer){
    int r = kernelSize/2;
    // Go through 2D map
    for(int h = 0; h < buffer->height; h++){
        for(int w = 0; w < buffer->width; w++){
            float filteredWeight = 0;

            // Convolute with kernel
            for(int y = 0; y < kernelSize; y++){
                for(int x = 0; x < kernelSize; x++){
                    int u = w-r+x;
                    int v = h-r+y;
                    if(!outOfBounds(buffer,u,v))
                        filteredWeight += (buffer->get(u,v) * kernelGet(x,y));
                }
            }
            // Set new value
            filteredMap->insert(w,h,filteredWeight/16);
        }
    }

    Map<float> *tmp = filteredMap;
    filteredMap = buffer;
    return tmp;
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
    return float(int3x3kernel[index]);
}

void Filter::kernelSet(int x, int y, float value){
    int index = x + y*kernelSize;
    kernel[index] = value;
}
