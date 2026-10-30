#include "graph.hpp"
#include <queue>
#include <unordered_map>

bool topologicalOrder(const std::vector<Topic>& topics, std::vector<int>& order) {
    order.clear();
    const int n = (int)topics.size();
    std::unordered_map<int, int> idx;  // topic id -> position in vector
    for (int i = 0; i < n; i++) idx[topics[i].id] = i;

    std::vector<int> indeg(n, 0);
    std::vector<std::vector<int>> adj(n);  // prereq -> dependents
    for (int i = 0; i < n; i++)
        for (int p : topics[i].prereqs) {
            auto it = idx.find(p);
            if (it == idx.end()) return false;  // unknown prereq
            adj[it->second].push_back(i);
            indeg[i]++;
        }

    std::queue<int> q;
    for (int i = 0; i < n; i++) if (indeg[i] == 0) q.push(i);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(topics[u].id);
        for (int v : adj[u]) if (--indeg[v] == 0) q.push(v);
    }
    return (int)order.size() == n;  // fewer than n means a cycle
}
