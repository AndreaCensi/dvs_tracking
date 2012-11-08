#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QString>
#include <QList>
#include "event.h"

class Logger
{
public:
    Logger();
    void log(Event *e);
    void saveToFile(QString filename);
    int getInterval();
    bool done();

private:
    struct Log{
        QList<unsigned int> timeStamp;
        QList<int> x;
        QList<int> y;
        QList<int> type;
    };
    Log eventLog;
    bool finished;
};

#endif // LOGGER_H
