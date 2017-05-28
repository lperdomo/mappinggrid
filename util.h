#ifndef UTIL_H
#define UTIL_H

#include <cmath>

class Util
{
public:
    static bool isAngleAtRange(double angle1, double angle2, double range);
    static double distanceBetweenPoints(double x1, double y1, double x2, double y2, double width, double height);
    static double angleBetweenPoints(double x1, double y1, double x2, double y2, double width, double height);
private:
    Util();
};

#endif // UTIL_H
