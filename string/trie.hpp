#ifndef _TRIE_HPP_
#define _TRIEL_HPP_

#include <array>
#include <vector>

struct Trie {
    struct Node {
        std::array<int,26> ch;
        int cnt;            // この頂点に対応する文字列の数
        int sub;            // 部分木に含まれる文字列の数
        Node () : cnt(0), sub(0) {
            std::fill(ch.begin(), ch.end(), -1);
        }
    };

    std::vector<Node> nodes;

    Trie() : nodes(1) {}

    void insert(const std::string &s) {
        int t = 0;
        for (int i = 0; i < (int)s.size(); i++) {
            if (nodes[t].ch[s[i]-'a'] == -1) {
                nodes[t].ch[s[i]-'a'] = nodes.size();
                nodes.push_back(Node());
            }
            nodes[t].sub++;
            t = nodes[t].ch[s[i]-'a'];
        }
        nodes[t].sub++;
        nodes[t].cnt++;
    }
};

#endif
