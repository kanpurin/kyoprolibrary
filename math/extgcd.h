#ifndef _EXTGCD_H_
#define _EXTGCD_H_

// ax + by = gcd(a,b)
template<typename T>
T extgcd(T a, T b, T &x, T &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    T d = extgcd(b, a%b, y, x);
    y -= a / b * x;
    return d;
}

#endif
