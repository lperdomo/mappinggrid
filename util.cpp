#include "util.h"

Util::Util()
{
}

bool Util::isAngleAtRange(double angle1, double angle2, double range)
{
    int difference = angle1 - angle2;
    if (std::abs(difference % 360) <= range || 360 - std::abs(difference % 360) <= range) {
        return true;
    }
    return false;
}
