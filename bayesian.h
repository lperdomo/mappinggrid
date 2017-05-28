#ifndef BAYESIAN_H
#define BAYESIAN_H

#include <iostream>

class Bayesian
{
public:
    Bayesian(double empty, double occupied);
    double getEmpty();
    double getOccupied();
    void setEmptyRead(double emptyRead);
    void setOccupiedRead(double occupiedRead);
    void calculate();
    static double min;
    static double max;
private:
    double empty;
    double occupied;
    double emptyRead;
    double occupiedRead;
};

#endif // BAYESIAN_H
