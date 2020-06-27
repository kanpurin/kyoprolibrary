#ifndef _KMP_H_
#define _KMP_H_

#include <vector>

// KMP法 O(N)
// verify : https://atcoder.jp/contests/abc135/tasks/abc135_f
template<typename T>
class KMP {
public:
    std::vector<T> pattern;
    int plen;
    std::vector<int> table;

    // tからsを探す(sを引数に)
    KMP(const std::vector<T>& s) : pattern(s), plen((int)pattern.size()), table(plen + 1) {
        table[0] = -1;
        int j = -1;
        for (int i = 0; i < plen; i++) {
            while (j >= 0 && pattern[i] != pattern[j]) j = table[j];
            j++;
            if (i + 1 < plen && pattern[i + 1] == pattern[j]) table[i + 1] = table[j];
            else table[i + 1] = j;
        }
    }

    // マッチする左端の箇所
    std::vector<int> search(const std::vector<T>& text) {
        std::vector<int> res;
        int head = 0, j = 0, tlen = (int)text.size();
        while (head + j < tlen) {
            if (pattern[j] == text[head + j]) {
                if (++j != plen) continue;
                res.push_back(head);
            }
            head += j - table[j], j = std::max(table[j], 0);
        }
        return res;
    }
};

#endif
