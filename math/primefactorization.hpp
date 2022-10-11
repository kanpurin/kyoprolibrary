#ifndef _PRIME_FACTORIZATION_HPP_
#define _PRIME_FACTORIZATION_HPP_

#include <vector>
#include <algorithm>

#include "isprime.hpp"

// 素因数分解 O(N^(1/4))
// (素因数,個数)
long long PollardRho(long long N) {
    if (N % 2 == 0) return 2;
    long long m = 1LL<<(70-__builtin_clrsbll(N))/8;
    for (long long c = 1; c < N; c++) {
        auto f = [&](long long x) { return ((__int128_t)x*x+c)%N; };
        long long x,y,g,q,r,k,ys;
        y = k = 0;
        g = q = r = 1;
        while (g == 1) {
            x = y;
            while(k < 3*r/4) {
                y = f(y);
                k += 1;
            }
            while(k < r && g == 1) {
                ys = y;
                for (int _ = 0; _ < std::min(m,r-k); _++) {
                    y = f(y);
                    q = (__int128_t)q*abs(x-y)%N;
                }
                g = std::__gcd(q,N);
                k += m;
            }
            k = r;
            r <<= 1;
        }
        if (g == N) {
            g = 1;
            y = ys;
            while(g == 1) {
                y = f(y);
                g = std::__gcd((long long)abs(x-y),N);
            }
        }
        if (g == N) continue;
        if (isprime(g)) return g;
        else if (isprime(N/g)) return N/g;
        else return PollardRho(g);
    }
    return -1; // error
}

std::vector<std::pair<long long,int>> prime_factorization(long long N) {
    std::vector<std::pair<long long,int>> res;
    while(!isprime(N) && N > 1) {
        long long p = PollardRho(N);
        int cnt = 0;
        while(N%p==0) {
            N /= p;
            cnt++;
        }
        res.push_back({p,cnt});
    }
    if (N > 1) res.push_back({N,1});
    sort(res.begin(), res.end());
    return res;
}

#endif
