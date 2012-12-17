#ifndef FILTER_H
#define FILTER_H

#include "map.h"

class Filter
{
public:
    Filter(int size, float standardDeviation, int mapW, int mapH);
    ~Filter();
    Map<int>* smoothen(Map<int> *buffer);

    static const float PI;

private:
    //operations on kernel
    int kernelGet(int x, int y);
    void kernelSet(int x, int y, float value);
    void generateKernel();
    float convolute();
    bool outOfBounds(Map<int> *buffer, int x, int y);

    Map<int> *filteredMap;
    int kernelSize;
    float sd;
    float *kernel;
};

#endif // FILTER_H
