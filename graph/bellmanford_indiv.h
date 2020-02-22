#ifndef _BELLMAN_FORD_INDIVIDUAL_H_
#define _BELLMAN_FORD_INDIVIDUAL_H_

#include <vector>
#include <limits>

// bellman-ford法
struct edge { int to, cost; };

// sからののコスト
// いくらでも小さくできるものはd[] = -LLINF
void bellman_ford(int s, const std::vector< std::vector< edge > > &G, std::vector< long long > &d) {
    const int V = G.size();
    const long long LLINF = 9223372036854775807;
    d = std::vector< long long >(V, LLINF);
    d[s] = 0;
    bool update = true;
    int i = 0;
    while (update)
    {
        update = false;
        for (int j = 0; j < V; j++)
        {
            for (const edge &e : G[j])
            {
                if (d[j] != -LLINF && d[j] != LLINF && d[j] + e.cost < d[e.to]) {
                    d[e.to] = d[j] + e.cost;
                    if (i == V - 1) {
                        d[e.to] = -LLINF;
                    }
                    update = true;
                }
                else if (d[j] == -LLINF && d[e.to] != -LLINF) {
                    d[e.to] = -LLINF;
                    update = true;
                }
            }
        }
        if (i < V - 1) i++;
    }
}

#endif
