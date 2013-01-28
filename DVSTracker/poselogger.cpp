#include "poselogger.h"

#include <QFile>

PoseLogger::PoseLogger(QString filename)
{
    outputFile = filename;
    finished = false;
    numPoses = 0;
    limit = 10000;
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
                     double rx, double ry, double rz,
                     float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3,
                     double ts){
    if(finished)
        return;

//    if(numPoses > limit){
//        stop();
//        return;
//    }

    QString sx = QString::number(x);
    QString sy = QString::number(y);
    QString sz = QString::number(z);

    QString srx = QString::number(rx);
    QString sry = QString::number(ry);
    QString srz = QString::number(rz);

    QString sx0 = QString::number(x0);
    QString sy0 = QString::number(y0);
    QString sx1 = QString::number(x1);
    QString sy1 = QString::number(y1);
    QString sx2 = QString::number(x2);
    QString sy2 = QString::number(y2);
    QString sx3 = QString::number(x3);
    QString sy3 = QString::number(y3);

    QString t = QString::number(ts);

    output += sx + "\t" + sy +  "\t" + sz + "\t"
            + srx + "\t" + sry + "\t" + srz + "\t"
            + sx0 + "\t" + sy0 +  "\t"
            + sx1 + "\t" + sy1 +  "\t"
            + sx2 + "\t" + sy2 +  "\t"
            + sx3 + "\t" + sy3 +  "\t"
            + t
            + "\n";

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
