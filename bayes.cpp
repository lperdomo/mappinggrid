#include "bayes.h"

Bayes::Bayes(double empty, double occupied)
{
    this->empty = empty;
    this->occupied = occupied;
}

void Bayes::setEmpty(double empty)
{
    if (empty > 0 && empty < 1) {
        this->empty = empty;
        this->occupied = 1.0-this->empty;
    }
}

double Bayes::getEmpty()
{
    return empty;
}

void Bayes::setOccupied(double occupied)
{
    if (occupied > 0 && occupied < 1) {
        this->occupied = occupied;
        this->empty = 1.0-this->occupied;
    }
}

double Bayes::getOccupied()
{
    return occupied;
}
