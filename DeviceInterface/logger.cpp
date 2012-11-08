#include "logger.h"
#include <QFile>
#include <QTextStream>

Logger::Logger()
{
    eventLog.timeStamp.reserve(1000000);
    eventLog.x.reserve(1000000);
    eventLog.y.reserve(1000000);
    eventLog.type.reserve(1000000);
}

void Logger::log(Event *e){
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
}
