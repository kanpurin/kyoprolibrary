#ifndef _ERATOSTHENES_H_
#define _ERATOSTHENES_H_

#include <vector>

// エラトステネスの篩 O(NloglogN)
// N以下の素数
struct Eratosthenes {
private:
    int N;

public:
    std::vector< bool > isprime;
    std::vector< int > prime;

    Eratosthenes(int n) : N(n) {
        isprime.resize(N + 1, true);
        isprime[0] = isprime[1] = false;
        for (int i = 2; i * i <= N; i++) {
            if (isprime[i]) {
                for (int j = i + i; j <= N; j += i) {
                    isprime[j] = false;
                }
            }
        }
        for (int i = 2; i <= N; i++) {
            if (isprime[i]) {
                prime.push_back(i);
            }
        }
    }
};

#endif
