#ifndef POSELOGGER_H
#define POSELOGGER_H

#include <QString>
#include <QTextStream>

//! A Logger for pose estimation data
/*!
    Stores translation, rotation, LED positions on the camera image as well as their timestamps to a text file.
    \author Jonas Strubel
*/
class PoseLogger
{
public:
    PoseLogger(QString filename);
    void log(double x, double y, double z,
             double rx, double ry, double rz,
             float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3,
             double ts);
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
