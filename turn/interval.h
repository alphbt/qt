#ifndef INTERVAL_H
#define INTERVAL_H

#include <iostream>

class point2;

class interval_t
{
    double _l;
    double _h;
public:
    interval_t() : _l(0.0), _h(0.0) {}
    interval_t(double x = 0) : _l(x), _h(x) {}
    interval_t(double l , double h) :_l(l), _h(h)
    {
        if (_h < _l) throw "error";
    }
    interval_t(const point2& p);
    interval_t(const interval_t& x) : _l(x._l), _h(x._h) {}

    interval_t& operator=(double x);
    interval_t& operator=(const interval_t& x);
    bool operator>(double x);
    const interval_t operator-(const interval_t& x);
    const interval_t& operator*(double p);
    const interval_t& operator/(double p);

    double l() const;
    double h() const;

    friend interval_t operator+(const interval_t& x, const interval_t& y);
    friend interval_t operator-(const interval_t& x, const interval_t& y);
    friend interval_t operator*(const interval_t& x, const interval_t& y);
    friend interval_t operator/(const interval_t& x, const interval_t& y);
    friend std::ostream& operator<<(std::ostream& out, const interval_t& x);
};

    bool zero_in(const interval_t& p);

#endif // INTERVAL_H
