#include <iostream>
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <iomanip>
#include "analysis.h"
#include "shortest_path.h"

// ── Degree of separation ─────────────────────────────────────────────────────

int Analysis::degreeOfSeparation(const Graph& g, int userId1, int userId2) {
    PathResult result = ShortestPath::bfsShortestPath(g, userId1, userId2);
    if (!result.found) return -1; // not connected
    return (int)result.path.size() - 1;
}

// ── Mutual friends ───────────────────────────────────────────────────────────

std::vector<int> Analysis::mutualFriends(const Graph& g, int userId1, int userId2) {
    // Build neighbour sets
    auto buildSet = [&](int uid) {
        std::unordered_set<int> s;
        for (const auto& e : g.getNeighbors(uid))
            s.insert(e.getTo());
        return s;
    };

    auto s1 = buildSet(userId1);
    auto s2 = buildSet(userId2);

    std::vector<int> common;
    for (int id : s1)
        if (s2.count(id)) common.push_back(id);

    std::sort(common.begin(), common.end());
    return common;
}

// ── Most connected user ──────────────────────────────────────────────────────

User Analysis::mostConnectedUser(const Graph& g) {
    const auto& users = g.getAllUsers();
    if (users.empty())
        throw std::runtime_error("Graph is empty.");

    int    bestId  = -1;
    int    bestDeg = -1;

    for (const auto& [uid, _] : users) {
        int deg = (int)g.getNeighbors(uid).size();
        if (deg > bestDeg) {
            bestDeg = deg;
            bestId  = uid;
        }
    }
    return g.getUser(bestId);
}

// ── Is graph connected (BFS from first node) ─────────────────────────────────

bool Analysis::isGraphConnected(const Graph& g) {
    const auto& users = g.getAllUsers();
    if (users.empty()) return true;

    int startId = users.begin()->first;
    std::unordered_set<int> visited;
    std::queue<int>         q;

    visited.insert(startId);
    q.push(startId);

    while (!q.empty()) {
        int cur = q.front(); q.pop();
        for (const auto& e : g.getNeighbors(cur)) {
            int nb = e.getTo();
            if (!visited.count(nb)) {
                visited.insert(nb);
                q.push(nb);
            }
        }
    }
    return (int)visited.size() == g.getUserCount();
}

// ── Degree ───────────────────────────────────────────────────────────────────

int Analysis::degree(const Graph& g, int userId) {
    return (int)g.getNeighbors(userId).size();
}

// ── Reachable users (BFS) ────────────────────────────────────────────────────

std::vector<int> Analysis::reachableUsers(const Graph& g, int userId) {
    std::unordered_set<int> visited;
    std::queue<int>         q;

    visited.insert(userId);
    q.push(userId);

    while (!q.empty()) {
        int cur = q.front(); q.pop();
        for (const auto& e : g.getNeighbors(cur)) {
            int nb = e.getTo();
            if (!visited.count(nb)) {
                visited.insert(nb);
                q.push(nb);
            }
        }
    }
    visited.erase(userId); // exclude self
    return std::vector<int>(visited.begin(), visited.end());
}

// ── Influence score ──────────────────────────────────────────────────────────

double Analysis::influenceScore(const Graph& g, int userId) {
    double score = 0.0;
    for (const auto& e : g.getNeighbors(userId))
        score += e.getWeight();
    return score;
}

// ── Network summary ──────────────────────────────────────────────────────────

void Analysis::networkSummary(const Graph& g) {
    std::cout << "\n======= Network Summary =======\n";
    std::cout << "Total users      : " << g.getUserCount() << "\n";
    std::cout << "Total connections: " << g.getEdgeCount()  << "\n";
    std::cout << "Graph connected  : " << (isGraphConnected(g) ? "Yes" : "No") << "\n";

    User top = mostConnectedUser(g);
    std::cout << "Most connected   : " << top.getName()
              << " (" << degree(g, top.getId()) << " connections)\n";

    // Average degree
    double totalDeg = 0;
    for (const auto& [uid, _] : g.getAllUsers())
        totalDeg += degree(g, uid);
    double avg = g.getUserCount() > 0 ? totalDeg / g.getUserCount() : 0;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Average degree   : " << avg << "\n";
    std::cout << "================================\n";
}
