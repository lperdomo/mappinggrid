#include "histogramic.h"

int Histogramic::max = 15;
int Histogramic::min = 0;

Histogramic::Histogramic()
{
    cv = -1;
}

int Histogramic::getCV()
{
    return cv;
}

void Histogramic::setCV(int cv)
{
    this->cv = cv;
}

void Histogramic::addCV()
{
    cv += 3;
    if (cv > Histogramic::max) cv = Histogramic::max;
}

void Histogramic::subCV()
{
    cv -= 1;
    if (cv < Histogramic::min) cv = Histogramic::min;
}

double Histogramic::proportionalCV()
{
    return (cv > 0 ? ((float)cv/(float)(Histogramic::max-Histogramic::min)) : 0);
}
