#ifndef _SUFFIX_ARRAY_H_
#define _SUFFIX_ARRAY_H_

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <numeric> // for iota
#include "sparsetable.h"

struct SuffixArray {
private:
    std::vector< int > SA, LCP;
    std::vector<int> rank;// rank[i]:s[i:]のSAのindex
    std::string s;
    SparseTable<int> st;

    // 比較関数 s[si:] < t[ti:]
    bool lt_substr(std::string& t, int si = 0, int ti = 0)
    {
        int sn = s.size(), tn = t.size();
        while (si < sn && ti < tn) {
            if (s[si] < t[ti]) return (true);
            if (s[si] > t[ti]) return (false);
            ++si, ++ti;
        }
        return (si >= sn && ti < tn);
    }

    // prefixがtの辞書順最小
    // O(|t|log|S|)
    int lower_bound(std::string& t)
    {
        int low = -1, high = SA.size();
        while (high - low > 1) {
            int mid = (low + high) >> 1;
            if (lt_substr(t, SA[mid])) low = mid;
            else high = mid;
        }
        return high;
    }

    // [first,second) のprefixがt
    // O(|t|log|S|)
    std::pair<int, int> lower_upper_bound(std::string& t) {
        int idx = lower_bound(t);
        int low = idx - 1, high = SA.size();
        t.back()++;
        while (high - low > 1) {
            int mid = (low + high) >> 1;
            if (lt_substr(t, SA[mid])) low = mid;
            else high = mid;
        }
        t.back()--;
        return {idx, high};
    }
public:

    SuffixArray(const std::string& str) : s(str)
    {
        SA.resize(s.size());
        iota(begin(SA), end(SA), 0);
        sort(begin(SA), end(SA), [&](const int& a, const int& b)
        {
            if (s[a] == s[b]) return(a > b);
            return (s[a] < s[b]);
        });
        std::vector< int > classes(s.size()), c(s.size()), cnt(s.size());
        for (int i = 0; i < (int)s.size(); i++) {
            c[i] = s[i];
        }
        for (int len = 1; len < (int)s.size(); len <<= 1) {
            for (int i = 0; i < (int)s.size(); i++) {
                if (i > 0 && c[SA[i - 1]] == c[SA[i]] && SA[i - 1] + len < (int)s.size() && c[SA[i - 1] + len / 2] == c[SA[i] + len / 2]) {
                    classes[SA[i]] = classes[SA[i - 1]];
                }
                else {
                    classes[SA[i]] = i;
                }
            }
            iota(begin(cnt), end(cnt), 0);
            copy(begin(SA), end(SA), begin(c));
            for (int i = 0; i < (int)s.size(); i++) {
                int s1 = c[i] - len;
                if (s1 >= 0) SA[cnt[classes[s1]]++] = s1;
            }
            classes.swap(c);
        }
    }

    int count(std::string &t) {
        std::pair<int, int> p = lower_upper_bound(t);
        return p.second - p.first;
    }

    // s中のtの位置 
    // 順番は適当 ソートしてない
    std::vector<int> find(std::string &t) {
        std::vector<int> v;
        std::pair<int, int> p = lower_upper_bound(t);
        for(int i = p.first; i < p.second; i++) {
            v.push_back(SA[i]);
        }
        return v;
    }

    void print_sa() {
        for(int i = 0; i < (int)s.size(); i++) std::cout << i << ": " << s.substr(SA[i]) << std::endl;
    }

    // O(|S|)
    void build_lcp() {
        rank.resize(s.size());
        for(int i = 0; i < (int)s.size(); i++) rank[SA[i]] = i;
        LCP.resize(s.size());
        LCP[0] = 0;
        for (int i = 0, h = 0; i < (int)s.size(); i++) {
            if (rank[i] + 1 < (int)s.size()) {
                for (int j = SA[rank[i] + 1]; std::max(i, j) + h < s.length() && s[i + h] == s[j + h]; ++h);
                LCP[rank[i] + 1] = h;
                if (h > 0) --h;
            }
        }
        st = SparseTable<int>(LCP, [](int x, int y) {return std::min(x, y); });
    }

    void print_lcp() {
        std::cout << "No.\tLCP\tsuffix" << std::endl;
        std::cout << "----------------------" << std::endl;
        for(int i = 0; i < (int)s.size(); i++) std::cout << i << ":\t" << LCP[i] << "\t" << s.substr(SA[i]) << std::endl;
    }

    // s[a:]とs[b:]のLCP
    int get_lcp(int a, int b) {
        if (a == b) return s.substr(a).size();
        return st.rmq(std::min(rank[a], rank[b]) + 1, std::max(rank[a], rank[b]) + 1);
    }
};

#endif
