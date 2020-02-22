#ifndef _ROLLING_HASH_H_
#define _ROLLING_HASH_H_

#include <vector>
#include <string>

// ローリングハッシュ
struct RollingHash {
private:
    static const int base1 = 1007, base2 = 2009;
    static const int mod1 = 1000000007, mod2 = 1000000009;
    std::vector< long long > hash1, hash2, power1, power2;
public:
    // construct O(N)
    RollingHash(const std::string &S) {
        int n = (int)S.size();
        hash1.assign(n + 1, 0);
        hash2.assign(n + 1, 0);
        power1.assign(n + 1, 1);
        power2.assign(n + 1, 1);
        for (int i = 0; i < n; ++i) {
            hash1[i + 1] = (hash1[i] * base1 + S[i]) % mod1;
            hash2[i + 1] = (hash2[i] * base2 + S[i]) % mod2;
            power1[i + 1] = (power1[i] * base1) % mod1;
            power2[i + 1] = (power2[i] * base2) % mod2;
        }
    }

    // get hash of S[left:right] O(1)
    inline std::pair< long long, long long > get(int l, int r) const {
        long long res1 = hash1[r] - hash1[l] * power1[r - l] % mod1;
        if (res1 < 0) res1 += mod1;
        long long res2 = hash2[r] - hash2[l] * power2[r - l] % mod2;
        if (res2 < 0) res2 += mod2;
        return {res1, res2};
    }
};

#endif
