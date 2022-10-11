#ifndef _COMBINATION_ARBITRARY_H_
#define _COMBINATION_ARBITRARY_H_

#include <vector>
#include <tuple>
#include <cassert>

// comb 任意MOD (前計算O(MOD) クエリO(logN))
struct Combination {
private:
    int MOD;
    std::vector<std::tuple<int,int,int>> pf;
    std::vector<std::vector<int>> fact, ifact;

    long long extgcd(long long a, long long b, long long &x, long long &y) {
        long long g = a; x = 1, y = 0;
        if(b != 0) g = extgcd(b, a%b, y, x), y -= (a/b) * x;
        return g;
    }

    long long inv(long long a, long long n) {
        long long s, t;
        extgcd(a, n, s, t);
        return (n+s) % n;
    }

    constexpr long long _safe_mod(long long x, long long m) {
        x %= m;
        if (x < 0) x += m;
        return x;
    }

    long long powmod(long long k, long long n, long long mod) {
        long long x = 1;
        while (n > 0) {
            if (n & 1) {
                x = x * k % mod;
            }
            k = k * k % mod;
            n >>= 1;
        }
        return x;
    }

    constexpr std::pair<long long, long long> _inv_gcd(long long a, long long b) {
        a = _safe_mod(a, b);
        if (a == 0) return {b, 0};

        long long s = b, t = a;
        long long m0 = 0, m1 = 1;

        while (t) {
            long long u = s / t;
            s -= t * u;
            m0 -= m1 * u;

            auto tmp = s;
            s = t;
            t = tmp;
            tmp = m0;
            m0 = m1;
            m1 = tmp;
        }
        if (m0 < 0) m0 += b / s;
        return {s, m0};
    }

    std::pair<long long, long long> crt(const std::vector<long long>& r, const std::vector<long long>& m) {
        assert(r.size() == m.size());
        int n = int(r.size());

        long long r0 = 0, m0 = 1;
        for (int i = 0; i < n; i++) {
            assert(1 <= m[i]);
            long long r1 = _safe_mod(r[i], m[i]), m1 = m[i];
            if (m0 < m1) {
                std::swap(r0, r1);
                std::swap(m0, m1);
            }
            if (m0 % m1 == 0) {
                if (r0 % m1 != r1) return {0, 0};
                continue;
            }
            long long g, im;
            std::tie(g, im) = _inv_gcd(m0, m1);

            long long u1 = (m1 / g);
            if ((r1 - r0) % g) return {0, 0};

            long long x = (r1 - r0) / g % u1 * im % u1;

            r0 += x * m0;
            m0 *= u1;
            if (r0 < 0) r0 += m0;
        }
        return {r0, m0};
    }

    long long _comb(long long n, long long r, int id) {
        if(n < 0 || r < 0 || r > n) return 0;
        auto [p,q,pr] = pf[id];
        
        long long z = n-r;
        long long e0 = 0;
        for(long long u=n/p;u;u/=p) e0 += u;
        for(long long u=r/p;u;u/=p) e0 -= u;
        for(long long u=z/p;u;u/=p) e0 -= u;
        long long em = 0;
        for(long long u=n/pr;u;u/=p) em += u;
        for(long long u=r/pr;u;u/=p) em -= u;
        for(long long u=z/pr;u;u/=p) em -= u;

        long long ret = 1;
        while(n > 0) {
            ret = ret*fact[id][n%pr]%pr*ifact[id][r%pr]%pr*ifact[id][z%pr]%pr;
            n /= p; r /= p; z /= p;
        }
        (ret *= powmod(p, e0, pr)) %= pr;
        if(!(p==2 && q >= 3) && em%2) ret = (pr-ret) % pr;
        return ret;
    }
public:
    Combination(int MOD) : MOD(MOD) {
        int x = MOD;
        for (int i = 2; i <= MOD; i++) {
            if (x % i != 0) continue;
            int cnt = 0, pr = 1;
            while(x % i == 0) x/=i, cnt++, pr*=i;
            pf.push_back(std::make_tuple(i,cnt,pr));
            fact.push_back(std::vector<int>(MOD+1));
            ifact.push_back(std::vector<int>(MOD+1));
        }
        for (int i = 0; i < (int)pf.size(); i++) {
            auto [p,q,pr] = pf[i];
            fact[i][0] = ifact[i][0] = 1;
            for (int j = 1; j <= pr; j++) {
                if(j%p == 0) {
                    fact[i][j] = fact[i][j-1];
                } else {
                    fact[i][j] = (long long)fact[i][j-1] * j % pr;
                }
            }
            ifact[i][pr] = inv(fact[i][pr], pr);
            for (int j = pr-1; j >= 0; j--) {
                if((j+1)%p == 0) {
                    ifact[i][j] = ifact[i][j+1];
                } else {
                    ifact[i][j] = (long long)ifact[i][j+1]*(j+1) % pr;
                }
            }
        }
    }
    int comb(long long n, long long r) {
        std::vector<long long> _r(pf.size()), _m(pf.size());
        for (int i = 0; i < (int)pf.size(); i++) {
            _r[i] = _comb(n,r,i);
            _m[i] = std::get<2>(pf[i]);
        }
        return crt(_r,_m).first;
    }
};

#endif
