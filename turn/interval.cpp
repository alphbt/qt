#include"interval.h"
#include"point2.h"
#include<cmath>


interval_t::interval_t(const point2& p)
{
    if (p.x > p.y)
    {
        _l = p.y;
        _h = p.x;
    }
    else
    {
        _l = p.x;
        _h = p.y;
    }
}

interval_t& interval_t::operator=(double x)
{
    _l = _h = x;
    return *this;
}

double interval_t::l() const { return _l; }
double interval_t::h() const { return _h; }

bool interval_t::operator>(double x)
{
    if (_l > x && _h > x)
        return true;
    return false;
}

const interval_t interval_t::operator-(const interval_t& x)
{
    return interval_t(-x.h(), -x.l());
}

const interval_t& interval_t::operator*(double p)
{
    _l *= p;
    _h *= p;
    return *this;
}

const interval_t& interval_t::operator/(double p)
{
    _l /= p;
    _h /= p;
    return *this;
}

interval_t& interval_t::operator=(const interval_t& x)
{
    _l = x._l;
    _h = x._h;
    return *this;
}

interval_t operator+(const interval_t& x, const interval_t& y)
{
    return interval_t(x.l() + y.l(), x.h() + y.h());
}

interval_t operator-(const interval_t& x, const interval_t& y)
{
    return interval_t(x.l() - y.l(), x.h() - y.h());
}

interval_t operator*(const interval_t& x, const interval_t& y)
{
    double t1 = x.l() * y.l();
    double t2 = x.l() * y.h();
    double t3 = x.h() * y.l();
    double t4 = x.h() * y.h();

    return interval_t(__min(__min(t1, t2), __min(t3, t4)), __max(__max(t1, t2), __max(t3, t4)));
}

interval_t operator/(const interval_t& x, const interval_t& y)
{
    if (y.l() <= 0 && 0 <= y.h())
        throw "(interval_t) division by zero";

    return x * interval_t(1.0 / y.h(), 1.0 / y.l());
}

std::ostream& operator<<(std::ostream& out, const interval_t& x)
{
    out << "[" << x.l() << ";" << x.h() << "]";
    return out;
}

bool zero_in(const interval_t& p)
{

    if (p.l() <= 0 && p.h() >= 0)
        return true;
    return false;
}
