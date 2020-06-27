#ifndef _POW_H_
#define _POW_H_

//pow
template<typename T, typename U>
T pow(T k, U n, T unity = 1) {
    while (n > 0) {
        if (n & 1) {
            unity *= k;
        }
        k *= k;
        n >>= 1;
    }
    return unity;
}

#endif
