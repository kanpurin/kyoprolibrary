#ifndef _SQRT_INT_HPP_
#define _SQRT_INT_HPP_

// 0<=x*x<=N<=10^18 最大の整数x
long long sqrtint(long long N) {
    long long l = 0, r = 1000000001;
    while(r - l > 1) {
        long long mid = (l + r) / 2;
        if (mid*mid <= N) {
            l = mid;
        }
        else {
            r = mid;
        }
    }
    return l;
}

#endif
