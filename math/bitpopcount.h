#ifndef _BIT_POP_COUNT_H_
#define _BIT_POP_COUNT_H_

int bitpopcount(long long n) {
    int res = 0;
    while (n) {
        if (n & 1) res++;
        n >>= 1;
    }
    return res;
}

#endif
