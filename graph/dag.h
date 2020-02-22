#ifndef _DAG_H_
#define _DAG_H_

#include <vector>
#include <stack>

struct DAG {
private:
    struct Edge {
        int to;
    };
    std::vector< std::vector< Edge > > graph;
    bool is_dag = false;
    std::vector< int > sorted; // トポロジカルソート
    int V; // 頂点数
public:

    DAG(int v) {
        V = v;
        graph.resize(v);
    }

    // from から to への有向辺を張る
    void add_edge(int from, int to) {
        graph[from].push_back({to});
    }

    // トポロジカルソート O(V + E)
    // DAG じゃないなら size 0 の vectorを返す
    std::vector< int > topological_sort() {
        std::stack< int > sta;
        //std::vector<int> dist(V, 0);//その頂点までの最長路
        std::vector< int > in(V, 0);// 入次数
        int used_cnt = 0;//使用した頂点の数
        for (int i = 0; i < V; i++) {
            for (Edge e : graph[i]) {
                in[e.to]++;
            }
        }
        for (int i = 0; i < V; i++) if (in[i] == 0) {
            sta.push(i);
            used_cnt++;
        }
        while (!sta.empty()) {
            int p = sta.top(); sta.pop();
            sorted.push_back(p);
            for (Edge e : graph[p]) {
                int v = e.to;
                in[v]--;
                //dist[v] = std::max(dist[v], dist[p] + 1);
                if (in[v] == 0) {
                    sta.push(v);
                    used_cnt++;
                }
            }
        }
        if (used_cnt == V) {
            return sorted;
        }
        else {
            return std::vector< int >(0);
        }
    }

    std::vector< Edge >& operator[](int x) {
        return graph[x];
    }
};

#endif
