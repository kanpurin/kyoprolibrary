#ifndef _AHO_CORASICK_HPP_
#define _AHO_CORASICK_HPP_

#include <vector>
#include <queue>
#include "../string/trie.hpp"

struct AhoCorasick {
    Trie trie;
    std::vector<int> failure;
    std::vector<int> count; // マッチした文字列数
    AhoCorasick() {}

    void insert(const std::string &s) {
        trie.insert(s);
    }

    void build() {
        for (int c = 0; c < 26; c++) {
            if (trie.nodes[0].ch[c] != -1) continue;
            trie.nodes[0].ch[c] = 0;
        }
        failure.resize(trie.nodes.size(),-1);
        count.resize(trie.nodes.size(),0);
        std::queue<int> que;
        que.push(0);
        while(!que.empty()) {
            int v = que.front(); que.pop();
            for (int c = 0; c < 26; c++) {
                int to = trie.nodes[v].ch[c];
                if (to <= 0) continue;
                count[to] = trie.nodes[to].cnt;
                que.push(to);
                int now = v;
                while(now != 0 && trie.nodes[failure[now]].ch[c] == -1) {
                    now = failure[now];
                }
                failure[to] = (now==0?0:trie.nodes[failure[now]].ch[c]);
                count[to] += count[failure[to]];
            }
        }
    }

    // nodes[idx]からcで遷移した状態
    int move(int idx, int c) {
        if (trie.nodes[idx].ch[c] == -1) {
            return move(failure[idx],c);
        }
        else {
            return trie.nodes[idx].ch[c];
        }
    }
};

#endif
