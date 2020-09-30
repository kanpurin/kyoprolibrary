#ifndef _OSA_K_HPP_
#define _OSA_K_HPP_

#include <vector>
#include <cassert>

// osa_k法
// 前処理で素因数を持っておいて?素因数分解の高速化
// N以下の数の素因数分解がO(logN)で行える.構築O(NloglogN)
struct Osa_k {
private:
    std::vector<int> v;
public:
    Osa_k(int n) {
        assert(n >= 1);
        v.resize(n+1,-1);
        v[1] = 1;
        for(int i = 2; i <= n; i++) {
            if (v[i] != -1) continue;
            for(int j = i; j <= n; j += i) v[j] = i;
        }
    }

    // 素因数分解 素因数が降順に並ぶ
    std::vector<int> prime_factorization(int n) {
        assert(1 <= n && n < v.size());
        std::vector<int> ret;
        while(n > 1) ret.push_back(v[n]), n /= v[n];
        return ret;
    }
};

#endif
