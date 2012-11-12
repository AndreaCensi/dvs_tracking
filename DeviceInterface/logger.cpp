#include "logger.h"
#include <QFile>
#include <QTextStream>

#define INTERVAL 5000000

Logger::Logger()
{
    eventLog.reserve(1000000);
    finished = false;
}

void Logger::log(Event e){
    if(eventLog.empty())
        printf("Logging started...\n");
    eventLog.append(e);
}

void Logger::saveToFile(QString filename){
    printf("Saving...\n");
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream out(&file);
    for(int i = 0; i < eventLog.size(); i++){
        out << eventLog.at(i).timeStamp << "\t" << eventLog.at(i).posX << "\t" << eventLog.at(i).posY << "\t" << eventLog.at(i).polarity << "\n";   // Desired output
    }
    file.close();
        printf("Done.\n");
}

int Logger::getInterval(){
    if(eventLog.size() > 1)
        return eventLog.back().timeStamp-eventLog.at(0).timeStamp;
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

