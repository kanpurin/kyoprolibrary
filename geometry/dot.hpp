#ifndef _DOT_HPP_
#define _DOT_HPP_

#include "template.hpp"

// AB,CDの内積
coord_t dot(const Point &A, const Point &B, const Point &C, const Point &D) {
    return (B.x - A.x) * (D.x - C.x) + (B.y - A.y) * (D.y - C.y);
}

// AB,ACの内積
coord_t dot(const Point &A, const Point &B, const Point &C) {
    return dot(A,B,A,C);
}

// OA,OBの内積
coord_t dot(const Point &A, const Point &B) {
    return dot(Point(0,0),A,B);
}

#endif
