#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <limits>
#include <algorithm>
#include "shortest_path.h"

// ── PathResult::display ──────────────────────────────────────────────────────

void PathResult::display(const Graph& g) const {
    if (!found) {
        std::cout << "No path found.\n";
        return;
    }
    std::cout << "Path  : ";
    for (int i = 0; i < (int)path.size(); ++i) {
        std::cout << g.getUser(path[i]).getName();
        if (i + 1 < (int)path.size()) std::cout << " -> ";
    }
    std::cout << "\nHops  : " << (path.size() - 1)
              << "\nCost  : " << totalCost << "\n";
}

// ── Helper: reconstruct path from parent map ─────────────────────────────────

static std::vector<int> reconstructPath(
    const std::unordered_map<int,int>& parent, int src, int dst)
{
    std::vector<int> path;
    for (int cur = dst; cur != -1; cur = parent.at(cur))
        path.push_back(cur);
    std::reverse(path.begin(), path.end());
    if (path.front() != src) return {}; // disconnected
    return path;
}

// ── BFS (unweighted) ─────────────────────────────────────────────────────────

PathResult ShortestPath::bfsShortestPath(const Graph& g, int srcId, int dstId) {
    PathResult result;

    if (!g.hasUser(srcId) || !g.hasUser(dstId)) {
        std::cerr << "BFS Error: user not found.\n";
        return result;
    }
    if (srcId == dstId) {
        result.path  = {srcId};
        result.found = true;
        return result;
    }

    std::unordered_map<int,int>  parent;
    std::unordered_set<int>      visited;
    std::queue<int>              q;

    parent[srcId] = -1;
    visited.insert(srcId);
    q.push(srcId);

    while (!q.empty()) {
        int cur = q.front(); q.pop();
        if (cur == dstId) break;

        for (const auto& edge : g.getNeighbors(cur)) {
            int nb = edge.getTo();
            if (!visited.count(nb)) {
                visited.insert(nb);
                parent[nb] = cur;
                q.push(nb);
            }
        }
    }

    if (!parent.count(dstId)) return result; // not reachable

    result.path      = reconstructPath(parent, srcId, dstId);
    result.found     = !result.path.empty();
    result.totalCost = (double)(result.path.size() - 1); // hop count
    return result;
}

// ── Dijkstra (weighted) ──────────────────────────────────────────────────────

PathResult ShortestPath::dijkstraShortestPath(const Graph& g, int srcId, int dstId) {
    PathResult result;

    if (!g.hasUser(srcId) || !g.hasUser(dstId)) {
        std::cerr << "Dijkstra Error: user not found.\n";
        return result;
    }
    if (srcId == dstId) {
        result.path  = {srcId};
        result.found = true;
        return result;
    }

    // dist map
    const double INF = std::numeric_limits<double>::infinity();
    std::unordered_map<int,double> dist;
    std::unordered_map<int,int>    parent;

    for (const auto& [uid, _] : g.getAllUsers()) {
        dist[uid]   = INF;
        parent[uid] = -1;
    }
    dist[srcId] = 0.0;

    // min-heap: (cost, nodeId)
    using P = std::pair<double,int>;
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
    pq.push({0.0, srcId});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();

        if (d > dist[u]) continue; // stale entry
        if (u == dstId)  break;

        for (const auto& edge : g.getNeighbors(u)) {
            int    v   = edge.getTo();
            double nd  = dist[u] + edge.getWeight();
            if (nd < dist[v]) {
                dist[v]   = nd;
                parent[v] = u;
                pq.push({nd, v});
            }
        }
    }

    if (dist[dstId] == INF) return result; // not reachable

    result.path      = reconstructPath(parent, srcId, dstId);
    result.found     = !result.path.empty();
    result.totalCost = dist[dstId];
    return result;
}
