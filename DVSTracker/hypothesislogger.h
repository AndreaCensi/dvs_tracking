#ifndef HYPOTHESISLOGGER_H
#define HYPOTHESISLOGGER_H

#include <QString>

class HypothesisLogger
{
public:
    HypothesisLogger();
    void log(double timeStamp, int frequency, int numHypothesis,
             int hypothesisIndex, int x, int y, int weight);
    void saveToFile(QString filename);
    bool done();
private:
    QString output;
    bool finished;

};

#endif // HYPOTHESISLOGGER_H
