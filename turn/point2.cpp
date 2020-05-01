#include "point2.h"
#include "interval.h"

point2::point2(const interval_t& p)
{
    x = p.l();
    y = p.h();
}

point2& point2::operator=(const point2& p)
{
    x = p.x;
    y = p.y;
    return *this;
}

point2& point2::operator-()
{
    x = -x;
    y = -y;
    return *this;
}

const point2 operator-(const point2& p, const point2& q)
{
    return point2(p.x - q.x, p.y - q.y);
}

const point2 operator+(const point2& p, const point2& q)
{
    return point2(q.x + p.x, q.y + p.y);
}

point2 point2::operator*(const double d)
{
    return point2(x * d, y * d);
}

point2 point2::operator/(const double d)
{
    return point2(x / d, y / d);
}
