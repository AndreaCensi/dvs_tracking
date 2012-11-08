#include "logger.h"
#include <QFile>
#include <QTextStream>

#define INTERVAL 5000000

Logger::Logger()
{
    eventLog.timeStamp.reserve(1000000);
    eventLog.x.reserve(1000000);
    eventLog.y.reserve(1000000);
    eventLog.type.reserve(1000000);
    finished = false;
}

void Logger::log(Event *e){
    if(eventLog.timeStamp.empty())
        printf("logging started...\n");
    eventLog.timeStamp.append(e->timeStamp);
    eventLog.x.append(e->posX);
    eventLog.y.append(e->posY);
    eventLog.type.append(e->polarity);
}

void Logger::saveToFile(QString filename){
    printf("saving...\n");
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream out(&file);
    for(int i = 0; i < eventLog.timeStamp.size(); i++){
        out << eventLog.timeStamp.at(i) << "\t" << eventLog.x.at(i) << "\t" << eventLog.y.at(i) << "\t" << eventLog.type.at(i) << "\n";             // Desired output
    }
    file.close();
        printf("done.\n");
}

int Logger::getInterval(){
    if(eventLog.timeStamp.size() > 1)
        return eventLog.timeStamp.back()-eventLog.timeStamp.at(0);
    else
        return 0;
}

bool Logger::done(){
    if(getInterval() > INTERVAL){
        if(!finished){
            printf("logging done.\n");
            saveToFile("C:/Users/giselher/Documents/uzh/event_log.txt");
            finished = true;
        }
        return true;
    }
    else
        return false;
}

