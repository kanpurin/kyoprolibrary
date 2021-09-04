#ifndef _BINARY_TRIE_HPP_
#define _BINARY_TRIE_HPP_

#include <cassert>

struct BinaryTrie {
private:
    int bitlen = 30;
    using T = int;
    struct Node {
        int cnt;
        Node *ch[2];
        Node() : cnt(0), ch{nullptr,nullptr} {} 
    };
    Node *rt;
public:
    BinaryTrie(){rt=new Node();}

    int size() const {return rt->cnt;}

    // xを追加
    void insert(const T x) {
        Node *pt=rt;
        for (int i=bitlen-1; i >= 0; i--) {
            int y=x>>i&1;
            if (!pt->ch[y]) pt->ch[y]=new Node();
            pt->cnt++;
            pt=pt->ch[y];
        }
        pt->cnt++;
    }

    // xを削除
    // xが存在しないときなにもしない
    void erase(const T x) {
        if (count(x) == 0) return;
        Node *pt=rt;
        for (int i=bitlen-1; i>=0; i--) {
            pt->cnt--;
            pt=pt->ch[x>>i&1];
        }
        pt->cnt--;
    }

    // xをすべて削除
    // xが存在しないときなにもしない
    void eraseAll(const T x) {
        int c = count(x);
        if (c == 0) return;
        Node *pt=rt;
        for (int i=bitlen-1; i>=0; i--) {
            pt->cnt-=c;
            pt=pt->ch[x>>i&1];
        }
        pt->cnt-=c;
    }
    
    // xの個数
    int count(const T x) const {
        Node *pt=rt;
        for (int i=bitlen-1; pt&&i>=0; i--) {
            pt = pt->ch[x>>i&1];
        }
        if (!pt) return 0;
        return pt->cnt;
    }

    // kthmin(i^x) (1-indexed)
    int kth_min_element(int k, const T x) const {
        assert(0 < k && k <= size());
        Node *pt=rt;
        T ans = 0;
        for (int i=bitlen-1; i>=0; i--) {
            ans<<=1;
            int y = x>>i&1;
            if (pt->ch[y]&&pt->ch[y]->cnt>0) {
                if (pt->ch[y]->cnt>=k) pt=pt->ch[y];
                else k-=pt->ch[y]->cnt,pt=pt->ch[1-y],ans++;
            }
            else pt=pt->ch[1-y],ans++;
        }
        return ans;
    }

    // k以上の最小の要素は昇順何番目か(1-indexed)
    // k未満の個数+1
    int lower_bound(const T k, const T x) const {
        Node *pt=rt;
        int ans = 1;
        for (int i=bitlen-1; i>=0 && pt; i--) {
            int y = x>>i&1;
            if ((k>>i&1)&&pt->ch[y]) ans += pt->ch[y]->cnt;
            pt=pt->ch[(k>>i&1)^y];
        }
        return ans;
    }
};

#endif