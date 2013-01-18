#ifndef POSELOGGER_H
#define POSELOGGER_H

#include <QString>
#include <QTextStream>

class PoseLogger
{
public:
    PoseLogger(QString filename);
    void log(double x, double y, double z, double rx, double ry, double rz);
    void logTrackLost();
    void saveToFile(QString filename);
    bool done();
    void stop();

private:
    QString outputFile;
    QString output;
    bool finished;
    int numPoses;
    int limit;
};

#endif // POSELOGGER_H
