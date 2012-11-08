#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QList>
#include "event.h"

class Logger
{
public:
    Logger();
    void log(Event *e);
    void saveToFile(QString filename);
private:
    struct Log{
        QList<int> timeStamp;
        QList<int> x;
        QList<int> y;
        QList<int> type;
    };
    Log eventLog;
};

#endif // LOGGER_H
