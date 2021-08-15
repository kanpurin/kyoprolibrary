#ifndef _GEOMETRY_TEMPLATE_HPP_
#define _GEOMETRY_TEMPLATE_HPP_

using coord_t = int;

struct Point {
    coord_t x,y;
    Point():x(0),y(0){}
    Point(coord_t x, coord_t y):x(x),y(y){}
    bool operator<(const Point &p) const {
        return x < p.x || (x == p.x && y < p.y);
    }
};

#endif