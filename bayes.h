#ifndef BAYES_H
#define BAYES_H

#include <QTime>

#include <iostream>

class Bayes
{
public:
    Bayes(double empty, double occupied);
    void setEmpty(double empty);
    double getEmpty();
    void setOccupied(double occupied);
    double getOccupied();
    bool isTimeToScanAgain();
private:
    double empty;
    double occupied;
    QTime *scanTime;
};

#endif // BAYES_H
