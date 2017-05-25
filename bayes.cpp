#include "bayes.h"

Bayes::Bayes(double empty, double occupied)
{
    this->empty = empty;
    this->occupied = occupied;
    scanTime = NULL;
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
    scanTime = new QTime();
    if (occupied > 0 && occupied < 1) {
        this->occupied = occupied;
        this->empty = 1.0-this->occupied;
    }
}

double Bayes::getOccupied()
{
    return occupied;
}

bool Bayes::isTimeToScanAgain()
{
    if (scanTime) {
        std::cout << "olha" << scanTime->secsTo(QTime()) << std::endl;
        return (scanTime->secsTo(QTime()) > 5);
    }
    std::cout << "eita" << std::endl;
    return true;
}
