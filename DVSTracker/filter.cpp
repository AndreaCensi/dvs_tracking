#include "filter.h"


const float Filter::PI = 3.14159265f;

Filter::Filter(int size, float var)
{
    kernelSize = size;
    variance = var;
    kernel = new float[size*size];
    generateKernel();
}

Filter::~Filter(){
    delete [] kernel;
}

Map<float>* Filter::smoothen(Map<float> *buffer){
    Map<float> *filteredMap = new Map<filter>[buffer->height*buffer->width];

    // Go through 2D map
    for(int h = 0; h < buffer->height; h++){
        for(int w = 0; w < buffer->width; w++){
            float filteredWeight = 0;
            int r = kernelsize/2;

            // Convolute with kernel
            for(int y = 0; y < kernelSize; y++){
                for(int x = 0; x < kernelSize; x++){
                    int u = w-r+x;
                    int v = h-r+y;
                    if(outOfBounds(u,v))
                        filteredWeight += get(x,y) * 0.5;   //avg value???-----------------------
                    else
                        filteredWeight += get(x,y) * buffer->get(u,v);
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
    for(int y = -r; y <= y+r;y++){
        for(int x = -r; x <= x+r;x++){
            float value = float((1.0/(variance*variance*2*PI)*exp(-(x*x+y*y)/(2*pow(tDiff/variance),2))));
            set(x,y,value);
        }
    }

}

bool Filter::outOfBounds(Map<float> *buffer,int x, int y){
    if( x > 0 && y > 0 && x < buffer->width && y < buffer->height)
        return true;
    else
        return false;
}

float Filter::get(int x, int y){
    int index = x + y*kernelSize;
    return kernel[index];
}

float Filter::set(int x, int y, float value){
    int index = x + y*kernelSize;
    kernel[index] = value;
}
