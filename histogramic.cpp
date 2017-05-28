#include "histogramic.h"

int Histogramic::max = 15;
int Histogramic::min = 0;

Histogramic::Histogramic()
{
    cv = -1;
}

void Histogramic::addCV(int increment)
{
    cv += increment;
    if (cv > Histogramic::max) cv = Histogramic::max;
}

void Histogramic::subCV(int decrement)
{
    cv -= decrement;
    if (cv < Histogramic::min) cv = Histogramic::min;
}
