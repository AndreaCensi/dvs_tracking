#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QString>
#include <QList>
#include "event.h"

//! Legacy logger class
/*!
    Store event data in a text file.
    \author Jonas Strubel
*/
class Logger
{
public:
    Logger();
    void log(Event e);
    void saveToFile(QString filename);
    int getInterval();
    bool done();

private:
    QList<Event> eventLog;
    bool finished;

};

#endif // LOGGER_H
