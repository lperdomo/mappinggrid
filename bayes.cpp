#include "bayes.h"

Bayes::Bayes(double empty, double occupied)
{
    this->empty = empty;
    this->occupied = occupied;
}

void Bayes::setEmpty(double empty)
{
    this->empty = empty;
}

double Bayes::getEmpty()
{
    return empty;
}

void Bayes::setOccupied(double occupied)
{
    this->occupied = occupied;
}

double Bayes::getOccupied()
{
    return occupied;
}
