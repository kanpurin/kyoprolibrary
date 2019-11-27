#ifndef _KINDS_COUNTER_H_
#define _KINDS_COUNTER_H_

#include <map>

// 種類数カウント
template<typename T>
struct kinds_counter {
private:
    int cnt;
    //std::unordered_map<T,int> mp;
    std::map<T,int> mp;
public:
    kinds_counter() {
        cnt = 0;
    }

    // x を n 個追加
    void insert(T x, int n = 1) {
        mp[x] += n;
        if (mp[x] == 1) cnt++;
    }

    // x を n 個削除
    void erase(T x, int n = 1) {
        if (mp.find(x) != mp.end()) {
            if (mp[x] <= n && mp[x] > 0) {
                cnt--;
                mp[x] = 0;
            }
            else {
                mp[x] = max(0,mp[x] - n);
            }
        }
    }

    // 種類数
    int count() {
        return cnt;
    }

    // 全削除
    void clear() {
        cnt = 0;
        mp.clear();
    }
};

#endif
