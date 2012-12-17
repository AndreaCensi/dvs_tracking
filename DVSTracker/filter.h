#ifndef FILTER_H
#define FILTER_H

#include "map.h"
#include <QThread>
#include <QWaitCondition>
#include <QMutex>

class Filter : public QThread
{
public:
    Filter(int size, float standardDeviation, int mapW, int mapH, QObject *parent = 0);
    ~Filter();
    void smoothen(Map<int> *buf);
    Map<int>* getFilteredMap();
    void setMap(Map<int>* buf);
    bool isDone();

    //Threading
    void pause();
    void resume();
    void stop();
    void run();

    static const float PI;

private:
    //operations on kernel
    int kernelGet(int x, int y);
    void kernelSet(int x, int y, float value);
    void generateKernel();
    float convolute();
    bool outOfBounds(Map<int> *buf, int x, int y);
    void switchBuffer();

    int bufferIndex;

    Map<int> *filteredMap;
    Map<int> **outputBuffers;
    Map<int> *inputBuffer;

    int kernelSize;
    float sd;
    float *kernel;
    bool done;

    //threading
    QWaitCondition waitCond;
    QMutex mutex;
    bool paused;
    bool exit;
};

#endif // FILTER_H
