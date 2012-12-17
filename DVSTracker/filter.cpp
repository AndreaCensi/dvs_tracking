#include "filter.h"
#include <math.h>

#define NUM_BUFFERS 3

const float Filter::PI = 3.14159265f;

static int int3x3kernel[9] = {0,1,0,1,2,1,0,1,0}; // bit shift values

Filter::Filter(int size, float standardDeviation, int mapW, int mapH, QObject *parent) : QThread(parent)
{
    outputBuffers = new Map<int>*[NUM_BUFFERS];
    for(int i = 0; i < NUM_BUFFERS; i++)
        outputBuffers[i] = new Map<int>(mapW,mapH);

    bufferIndex = 0;
    filteredMap = outputBuffers[bufferIndex];

    inputBuffer = 0;

    kernelSize = size;
    sd = standardDeviation;
    kernel = new float[size*size];

    //threading
    exit = false;
    paused = true;

    // smoothing status
    done = false;

    //generateKernel();
}

Filter::~Filter(){
    for(int i = 0; i < NUM_BUFFERS; i++)
        delete outputBuffers[i];
    delete [] outputBuffers;
    delete [] kernel;
}

void Filter::smoothen(Map<int> *buf){
    int r = kernelSize/2;

    // Go through 2D map
    for(int h = 0; h < buf->height; h++){
        for(int w = 0; w < buf->width; w++){
            int filteredWeight = 0;

            // Convolute with kernel
            for(int y = 0; y < kernelSize; y++){
                for(int x = 0; x < kernelSize; x++){
                    int u = w-r+x;
                    int v = h-r+y;
                    if(!outOfBounds(buf,u,v))
                        filteredWeight += (buf->get(u,v) << kernelGet(x,y));
                }
            }
            // Set new value
            filteredMap->insert(w,h,(filteredWeight >> 4));
        }
    }
    // clear buffer
    for(int i = 0; i < buf->size(); i++)
        buf->set(i,0);
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

bool Filter::outOfBounds(Map<int> *buf,int x, int y){
    if( x >= 0 && y >= 0 && x < buf->width && y < buf->height)
        return false;
    else
        return true;
}

void Filter::switchBuffer(){
    if(bufferIndex == NUM_BUFFERS-1)
        bufferIndex = 0;
    else
        bufferIndex++;
    filteredMap = outputBuffers[bufferIndex];
}

int Filter::kernelGet(int x, int y){
    int index = x + y*kernelSize;
    return int3x3kernel[index];
}

void Filter::kernelSet(int x, int y, float value){
    int index = x + y*kernelSize;
    kernel[index] = value;
}

Map<int>* Filter::getFilteredMap(){
    Map<int> *latest = filteredMap;
    switchBuffer(); //take next Buffer
    return latest;
}

void Filter::setMap(Map<int> *buf){
    inputBuffer = buf;
}

bool Filter::isDone(){
    return done;
}

void Filter::resume(){
    mutex.lock();
    paused = false;
    mutex.unlock();
    waitCond.wakeOne();
}

void Filter::pause(){
    mutex.lock();
    paused = true;
    mutex.unlock();
}

void Filter::stop(){
    exit = true;
}

void Filter::run(){
    while(!exit){
        mutex.lock();
        if(paused)
            waitCond.wait(&mutex);
        mutex.unlock();
        done = false;
        smoothen(inputBuffer);
        pause();
        done = true;
    }
}
