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

double Util::distanceBetweenPoints(double x1, double y1, double x2, double y2, double width, double height)
{
    return sqrt(pow((x1 - width) - (x2 - width), 2) + pow((height - y1) - (height - y2), 2));
}

double Util::angleBetweenPoints(double x1, double y1, double x2, double y2, double width, double height)
{
    return atan2((height - y1) - (height - y2), (x1 - width) - (x2 - width))*180/M_PI;
}
