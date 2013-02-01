#ifndef MUTEX_H
#define MUTEX_H

#include <QMutex>
#include <QWaitCondition>

extern QMutex mutex;
extern QWaitCondition newData;

#endif // MUTEX_H
