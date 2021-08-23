#ifndef _POLYNOMIAL_INTERPOLATINO_HPP_
#define _POLYNOMIAL_INTERPOLATINO_HPP_

#include <vector>

#include "../data_structure/mint.hpp"

const int MOD = 1e9 + 7;

// N点(x_i,y_i)を通るN-1次多項式f(x)のf(T)の値 O(N^2)
mint<MOD> polynomial_interpolation(const std::vector<mint<MOD>> &x, const std::vector<mint<MOD>> &y, const mint<MOD> &T) {
    assert(x.size() == y.size());
    int n = x.size();
    mint<MOD> ret = 0;
    for (int i = 0; i < n; i++) {
        mint<MOD> t = 1;
        for (int j = 0; j < n; j++) {
            if (i == j) continue;
            t *= T-x[j];
            t /= x[i]-x[j];
        }
        ret += t*y[i];
    }
    return ret;
}

// N点(x_i,y_i)を通るN-1次多項式f(x)のf(T)の値 O(NlogMOD)
// {x_i}は初項a公差dの等差数列
mint<MOD> polynomial_interpolation(const mint<MOD> &a, const mint<MOD> &d, std::vector<mint<MOD>> &y, const mint<MOD> &T) {
    int n = y.size();
    mint<MOD> ret = 0;
    std::vector<mint<MOD>> fl(n), fr(n), ftl(n), ftr(n);
    fl[n-1] = fr[0] = ftl[0] = ftr[n-1] = 1;
    for (int i = 1; i < n; i++) fl[n-1] *= d*i; fl[n-1] = fl[n-1].inv();
    for (int i = n-1; i > 0; i--) fl[i-1] = fl[i] * d*i;
    for (int i = 1; i < n; i++) fr[0] *= -d*i; fr[0] = fr[0].inv();
    for (int i = 0; i < n-1; i++) fr[i+1] = fr[i] * -d*(n-1-i);
    for (int i = 0; i < n-1; i++) ftl[i+1] = ftl[i] * (T-(a+d*i));
    for (int i = n-1; i > 0; i--) ftr[i-1] = ftr[i] * (T-(a+d*i));
    for (int i = 0; i < n; i++) ret += y[i]*fl[i]*fr[i]*ftl[i]*ftr[i];
    return ret;
}

#endif