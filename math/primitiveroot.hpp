#ifndef _PRIMITIVE_ROOT_HPP_
#define _PRIMITIVE_ROOT_HPP_

#include <random>
#include <ctime>

#include "primefactorization.hpp"
#include "powmod.hpp"

// 原始根 O(√p)
int primitiveroot(int p) {
    auto v = prime_factorization(p-1);
    srand(time(NULL));
    while(true) {
        int a = rand() % (p-2) + 1;
        for (auto q : v) {
            if (powMod(a,(p-1)/q.first,p) == 1) {
                a = -1;
                break;
            }
        }
        if (a == -1) continue;
        return a;
    }
}

#endif
