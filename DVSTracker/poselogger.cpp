#include "poselogger.h"

#include <QFile>

PoseLogger::PoseLogger(QString filename)
{
    outputFile = filename;
    finished = false;
    numPoses = 0;
    limit = 1000;
}

void PoseLogger::saveToFile(QString filename){
    printf("Saving...\n");
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream out(&file);
        out << output;
    file.close();
        printf("Done.\n");
}

void PoseLogger::log(double x, double y, double z,
                     double rx, double ry, double rz){
    if(finished)
        return;

    if(numPoses > limit){
        finished = true;
        return;
    }

    QString sx = QString::number(x);
    QString sy = QString::number(y);
    QString sz = QString::number(z);
    QString srx = QString::number(rx);
    QString sry = QString::number(ry);
    QString srz = QString::number(rz);

    output += sx + "\t" + sy +  "\t" + sz
               + "\t" + srx + "\t" + sry + "\t" + srz + "\n";

    numPoses++;
}

void PoseLogger::logTrackLost(){
    output += "%Track lost\n";
}

bool PoseLogger::done(){
    return finished;
}

void PoseLogger::stop(){
    finished = true;
    saveToFile(outputFile);
}
