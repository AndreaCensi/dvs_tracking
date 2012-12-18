#include "hypothesislogger.h"
#include <QFile>
#include <QTextStream>

HypothesisLogger::HypothesisLogger()
{

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

}
