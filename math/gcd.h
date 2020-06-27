#ifndef _GCD_H_
#define _GCD_H_

//gcd
template<typename T>
T gcd(T a, T b) {
    return b ? gcd(b, a%b) : a;
}

#endif
