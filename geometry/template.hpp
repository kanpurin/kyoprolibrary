#ifndef _GEOMETRY_TEMPLATE_HPP_
#define _GEOMETRY_TEMPLATE_HPP_

#include <iostream>

using coord_t = int;

struct Point {
    coord_t x,y;
    Point():x(0),y(0){}
    Point(coord_t x, coord_t y):x(x),y(y){}
    bool operator<(const Point &p) const {
        return x < p.x || (x == p.x && y < p.y);
    }
};


struct Vector {
private:
    // https://codeforces.com/blog/entry/72815?#comment-570942
    int quad(coord_t x, coord_t y) const {
        return ((x>=0)^(y>=0))|((y>=0)<<1);
    }
public:
    coord_t x,y;
    Vector():x(0),y(0){}
    Vector(coord_t x, coord_t y):x(x),y(y){}
    Vector(const Vector &v):x(v.x),y(v.y){}
    Vector operator-() const { return Vector() - *this; }
    Vector& operator+=(const Vector &v) {
        x += v.x;
        y += v.y;
        return *this;
    }
    Vector& operator-=(const Vector &v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }
    friend Vector operator+(const Vector& lv, const Vector& rv) {
        return Vector(lv) += rv;
    }
    friend Vector operator-(const Vector& lv, const Vector& rv) {
        return Vector(lv) -= rv;
    }
    // (-pi,pi]
    bool operator<(const Vector &v) const {
        return quad(x,y)==quad(v.x,v.y)?x*v.y>y*v.x:quad(x,y)<quad(v.x,v.y);
    }
    friend std::ostream &operator<<(std::ostream &os, const Vector &v) {
        return os << "[" << v.x << "," << v.y << "]";
    }
};

#endif