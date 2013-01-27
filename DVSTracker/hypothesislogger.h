#ifndef HYPOTHESISLOGGER_H
#define HYPOTHESISLOGGER_H

#include <QString>
#include <QTextStream>

//! Logger class
/*!
    Stores the highest weighst found in a FrequencyAccumulator to a text file.
    \author Jonas Strubel
*/
class HypothesisLogger
{
public:
    HypothesisLogger(QString filename);
    void log(double timeStamp, int frequency, int numHypothesis,
             int hypothesisIndex, int x, int y, int weight);
    void saveToFile(QString filename);
    bool done();
    void stop();

private:
    QString outputFile;
    QString output;
    bool finished;
};

#endif // HYPOTHESISLOGGER_H
