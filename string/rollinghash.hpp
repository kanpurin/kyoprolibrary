#ifndef _ROLLING_HASH_HPP_
#define _ROLLING_HASH_HPP_

#include <vector>
#include <string>
#include <random>
#include <ctime>

// ローリングハッシュ
struct RollingHash {
private:
    using ull = unsigned long long;
    static const ull _mod = 0x1fffffffffffffff;
    static ull _base;
    std::vector<ull> _hashed, _power;

    inline ull _mul(ull a, ull b) const {
        ull au = a >> 31;
        ull ad = a & ((1UL << 31) - 1);
        ull bu = b >> 31;
        ull bd = b & ((1UL << 31) - 1);
        ull mid = ad * bu + au * bd;
        ull midu = mid >> 30;
        ull midd = mid & ((1UL << 30) - 1);
        ull ans = au * bu * 2 + midu + (midd << 31) + ad * bd;
        
        ans = (ans >> 61) + (ans & _mod);
        if (ans >= _mod) ans -= _mod;
        return ans;
    }
public:
    RollingHash(const std::string &s) {
        long long n = s.size();
        _hashed.assign(n + 1, 0);
        _power.assign(n + 1, 0);
        _power[0] = 1;
        for(long long i = 0; i < n; i++) {
            _power[i + 1] = _mul(_power[i], _base);
            _hashed[i + 1] = _mul(_hashed[i], _base) + s[i];
            if(_hashed[i + 1] >= _mod) _hashed[i + 1] -= _mod;
        }
    }
    
    // a[l:r) O(1)
    ull get(long long l, long long r) const {
        ull ret = _hashed[r] + _mod - _mul(_hashed[l], _power[r - l]);
        if(ret >= _mod) ret -= _mod;
        return ret;
    }
    
    ull connect(ull h1, ull h2, long long h2len) const {
        ull ret = _mul(h1, _power[h2len]) + h2;
        if(ret >= _mod) ret -= _mod;
        return ret;
    }
    
    void connect(const std::string &s) {
        long long n = _hashed.size() - 1, m = s.size();
        _hashed.resize(n + m + 1);
        _power.resize(n + m + 1);
        for(long long i = n; i < n + m; i++) {
            _power[i + 1] = _mul(_power[i], _base);
            _hashed[i + 1] = _mul(_hashed[i], _base) + s[i - n];
            if(_hashed[i + 1] >= _mod) _hashed[i + 1] -= _mod;
        }
    }
    
    // LCP(a[l1:r1),b[l2:r2])) O(logN)
    long long LCP(const RollingHash &b, long long l1, long long r1, long long l2, long long r2) const {
        long long len = std::min(r1 - l1, r2 - l2);
        long long low = -1, high = len + 1;
        while(high - low > 1) {
            long long mid = (low + high) / 2;
            if(get(l1, l1 + mid) == b.get(l2, l2 + mid)) low = mid;
            else high = mid;
        }
        return low;
    }
};

std::mt19937_64 mt{(unsigned int)time(NULL)};
RollingHash::ull RollingHash::_base = mt() % RollingHash::_mod;

#endif