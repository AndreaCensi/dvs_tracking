#ifndef FILTER_H
#define FILTER_H

#include "map.h"

class Filter
{
public:
    Filter(int size, float standardDeviation);
    ~Filter();
    Map<float>* smoothen(Map<float> *buffer);

    static const float PI;

//private:
    //operations on kernel
    float kernelGet(int x, int y);
    void kernelSet(int x, int y, float value);
    void generateKernel();
    float convolute();
    bool outOfBounds(Map<float> *buffer, int x, int y);

    int kernelSize;
    float sd;
    float *kernel;

};

#endif // FILTER_H
