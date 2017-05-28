#include "bayesian.h"

double Bayesian::max = 0.9999;
double Bayesian::min = 0.0001;

Bayesian::Bayesian(double empty, double occupied)
{
    this->empty = empty;
    this->occupied = occupied;
    emptyRead = empty;
    occupiedRead = occupied;
}


double Bayesian::getEmpty()
{
    return empty;
}

double Bayesian::getOccupied()
{
    return occupied;
}

void Bayesian::setEmptyRead(double emptyRead)
{
    this->emptyRead = emptyRead;
    occupiedRead = 1.0-emptyRead;
}

void Bayesian::setOccupiedRead(double occupiedRead)
{
    this->occupiedRead = occupiedRead;
    emptyRead = 1.0-occupiedRead;
}

void Bayesian::calculate()
{
    occupied = (occupiedRead*occupied)/(occupiedRead*occupied + emptyRead*empty);
    //if (occupied > probMax) occupied = probMax;
    //else if (occupied < probMin) occupied = probMin;
    occupied = std::max(occupied, Bayesian::min);
    empty = std::max(1.0-occupied, Bayesian::min);

}
