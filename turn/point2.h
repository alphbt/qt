#ifndef POINT2_H
#define POINT2_H

#include "interval.h"

class point2
{
public:
    double x;
    double y;

    point2(double ix = 0, double iy = 0) : x(ix), y(iy) { }
    point2(const point2& p) : x(p.x), y(p.y) { }
    point2(const interval_t& p);

    point2& operator=(const point2& p);
    point2& operator-();
    friend const point2 operator-(const point2& p, const point2& q);
    friend const point2 operator+(const point2& p,const point2& q);
    point2 operator*(const double d);
    point2 operator/(const double d);
};


#endif // POINT2_H
