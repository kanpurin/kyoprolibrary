#ifndef _IS_PRIME_H_
#define _IS_PRIME_H_

//素数判定O(√n)
bool isprime(long long n) {
    if (n <= 1) return false;
    for (long long i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

#endif
