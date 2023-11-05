#ifndef _COMBINATION_ARBITRARY_H_
#define _COMBINATION_ARBITRARY_H_

#include <vector>
#include <tuple>
#include <algorithm>
#include <cassert>

// comb 任意MOD (前計算O(min(NlogM/loglogM,M)) クエリO(logNlogM/loglogM))
// https://nyaannyaan.github.io/library/modulo/arbitrary-mod-binomial.hpp.html
struct Combination {
private:
    using ll = long long;
    static const int N_MAX = 20000000;
    static const int MOD_MAX = (1<<30)-1;

    constexpr ll _safe_mod(ll x, ll m) {
        x %= m;
        if (x < 0) x += m;
        return x;
    }

    constexpr std::pair<ll, ll> _inv_gcd(ll a, ll b) {
        a = _safe_mod(a, b);
        if (a == 0) return {b, 0};

        ll s = b, t = a;
        ll m0 = 0, m1 = 1;

        while (t) {
            ll u = s / t;
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

    std::pair<ll, ll> crt(const std::vector<ll>& r, const std::vector<ll>& m) {
        assert(r.size() == m.size());
        int n = int(r.size());
        // Contracts: 0 <= r0 < m0
        ll r0 = 0, m0 = 1;
        for (int i = 0; i < n; i++) {
            assert(1 <= m[i]);
            ll r1 = _safe_mod(r[i], m[i]), m1 = m[i];
            if (m0 < m1) {
                std::swap(r0, r1);
                std::swap(m0, m1);
            }
            if (m0 % m1 == 0) {
                if (r0 % m1 != r1) return {0, 0};
                continue;
            }
            ll g, im;
            std::tie(g, im) = _inv_gcd(m0, m1);

            ll u1 = (m1 / g);
            if ((r1 - r0) % g) return {0, 0};

            ll x = (r1 - r0) / g % u1 * im % u1;

            r0 += x * m0;
            m0 *= u1;
            if (r0 < 0) r0 += m0;
        }
        return {r0, m0};
    }

    struct Barrett {
        using u32 = unsigned int;
        using i64 = long long;
        using u64 = unsigned long long;
        u32 m;
        u64 im;
        Barrett() : m(), im() {}
        Barrett(int n) : m(n), im(u64(-1) / m + 1) {}
        constexpr inline u64 mul(u64 a, u64 b) {
            u32 a1 = a>>32, a2 = a;
            u32 b1 = b>>32, b2 = b;
            u64 res = (u64)a1*b1+((u64)a2*b1>>32)+((u64)a1*b2>>32);
            if (a2*b1+a1*b2<a2*b1) res++;
            if((((u64)(a2*b1)+a1*b2)<<32)+(u64)a2*b2<(u64)a2*b2) res++;
            return res;
        }
        constexpr inline i64 quo(u64 n) {
            u64 x = mul(n,im);
            u32 r = n - x * m;
            return m <= r ? x - 1 : x;
        }
        constexpr inline i64 rem(u64 n) {
            u64 x = mul(n,im);
            u32 r = n - x * m;
            return m <= r ? r + m : r;
        }
        constexpr inline std::pair<i64, int> quorem(u64 n) {
            u64 x = mul(n,im);
            u32 r = n - x * m;
            if (m <= r) return {x - 1, r + m};
            return {x, r};
        }
        constexpr inline i64 pow(u64 n, i64 p) {
            u32 a = rem(n), r = m == 1 ? 0 : 1;
            while (p) {
            if (p & 1) r = rem(u64(r) * a);
            a = rem(u64(a) * a);
            p >>= 1;
            }
            return r;
        }
    };

    struct PrimePowerCombination {
    private:
        int p,q,MOD;
        std::vector<int> fact, ifact;
        int delta;
        Barrett bm, bp;

        ll lucas(ll n, ll r) {
            int res = 1;
            while(n) {
                int n0,r0;
                std::tie(n, n0) = bp.quorem(n);
                std::tie(r, r0) = bp.quorem(r);
                if (n0 < r0) return 0;
                res = bm.rem(1LL * res * fact[n0]);
                int buf = bm.rem(1LL * ifact[n0 - r0] * ifact[r0]);
                res = bm.rem(1LL * res * buf);
            }
            return res;
        }

    public:
        PrimePowerCombination(int _p, int _q) : p(_p), q(_q) {
            MOD = 1;
            while(_q--) MOD *= p;
            bm = Barrett(MOD), bp = Barrett(p);
            // enumerate
            int MX = std::min(MOD, N_MAX);
            fact.resize(MX);
            ifact.resize(MX);
            fact[0] = ifact[0] = 1;
            fact[1] = ifact[1] = 1;
            for (int i = 2; i < MX; i++) {
                if (i % p == 0) fact[i] = fact[i-1];
                else fact[i] = bm.rem(1LL * fact[i-1] * i);
            }
            ifact[MX-1] = bm.pow(fact[MX-1],MOD-MOD/p-1);
            for (int i = MX-2; i > 1; i--) {
                if ((i+1) % p == 0) ifact[i] = ifact[i+1];
                else ifact[i] = bm.rem(1LL * ifact[i+1] * (i+1));
            }
            delta = (p == 2 && q >= 3) ? 1 : MOD - 1;
        }

        ll comb(ll n, ll r) {
            if (n < 0 || r < 0 || r > n) return 0;
            if (q == 1) return lucas(n,r);
            ll m = n - r;
            int e0 = 0, eq = 0, i = 0;
            int res = 1;
            while (n) {
                res = bm.rem(1LL * res * fact[bm.rem(n)]);
                res = bm.rem(1LL * res * ifact[bm.rem(m)]);
                res = bm.rem(1LL * res * ifact[bm.rem(r)]);
                n = bp.quo(n);
                m = bp.quo(m);
                r = bp.quo(r);
                int eps = n - m - r;
                e0 += eps;
                if (e0 >= q) return 0;
                if (++i >= q) eq += eps;
            }
            if (eq & 1) res = bm.rem(1LL * res * delta);
            res = bm.rem(1LL * res * bm.pow(p, e0));
            return res;
        }
    };

public:
    int MOD;
    std::vector<int> M;
    std::vector<PrimePowerCombination> cs;

    Combination(ll mod) : MOD(mod) {
        assert(1 <= mod);
        assert(mod <= MOD_MAX);
        for (int i = 2; i * i <= mod; i++) {
            if (mod % i == 0) {
                int j = 0, k = 1;
                while(mod % i == 0) mod /= i, j++, k *= i;
                M.push_back(k);
                cs.emplace_back(i, j);
            }
        }
        if (mod == 1) return;
        M.push_back(mod);
        cs.emplace_back(mod, 1);
    }

    ll comb(ll n, ll r) {
        if (MOD == 1) return 0;
        std::vector<ll> rem, d;
        for (int i = 0; i < (int)cs.size(); i++) {
            rem.push_back(cs[i].comb(n, r));
            d.push_back(M[i]);
        }
        return crt(rem, d).first;
    }
};

#endif
