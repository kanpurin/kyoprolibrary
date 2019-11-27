#ifndef _MANACHER_H_
#define _MANACHER_H_

#include <vector>
#include <string>

// 文字列のみ
// iを中心とした最長回文の半径 O(N)
// 奇数長のみ (偶数は間に$を入れればいい)
std::vector< int > manacher(const std::string &s) {
    std::vector< int > radius(s.size());
    int i = 0, j = 0;
    while (i < (int)s.size()) {
        while (i - j >= 0 && i + j < (int)s.size() && s[i - j] == s[i + j]) {
            ++j;
        }
        radius[i] = j;
        int k = 1;
        while (i - k >= 0 && i + k < (int)s.size() && k + radius[i - k] < j) {
            radius[i + k] = radius[i - k];
            ++k;
        }
        i += k;
        j -= k;
    }
    return radius;
}

#endif
