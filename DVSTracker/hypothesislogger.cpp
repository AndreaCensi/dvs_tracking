#include "hypothesislogger.h"
#include <QFile>


HypothesisLogger::HypothesisLogger(QString filename)
{
    outputFile = filename;
    finished = false;
}

void HypothesisLogger::saveToFile(QString filename){
    printf("Saving...\n");
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream out(&file);
        out << output;
    file.close();
        printf("Done.\n");
}

void HypothesisLogger::log(double timeStamp, int frequency, int numHypothesis,
                           int hypothesisIndex, int x, int y, int weight){
    QString ts = QString::number(timeStamp);
    QString f = QString::number(frequency);
    QString nHyp = QString::number(numHypothesis);
    QString iHyp = QString::number(hypothesisIndex);
    QString posX = QString::number(x);
    QString posY = QString::number(y);
    QString w = QString::number(weight);
    output += ts + "\t" + f +  "\t" + nHyp
               + "\t" + iHyp + "\t" + posX + "\t" + posY + "\t"
               + w + "\n";
}

bool HypothesisLogger::done(){
    return finished;
}

void HypothesisLogger::stop(){
    finished = true;
    saveToFile(outputFile);
}
