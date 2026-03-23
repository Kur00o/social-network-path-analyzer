// ===== TEMP GRAPH (REMOVE LATER) =====
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <iostream>

struct Edge {
    int to;
    int weight;
};

class Graph {
public:
    std::unordered_map<int, std::vector<Edge>> adj;

    std::vector<Edge> getNeighbors(int id) const {
        if (adj.count(id))
            return adj.at(id);
        return {};
    }

    std::vector<int> getAllUserIds() const {
        std::vector<int> ids;
        for (auto &p : adj)
            ids.push_back(p.first);
        return ids;
    }
};
// ===== END TEMP GRAPH =====

#include "graph.cpp"
#include <vector>
#include <queue>
#include <unordered_set>

// Degree of Separation
int degreeOfSeparation(const Graph& g, int id1, int id2) {
    std::queue<std::pair<int, int>> q;
    std::unordered_set<int> visited;

    q.push({id1, 0});
    visited.insert(id1);

    while (!q.empty()) {
        auto front = q.front();
        q.pop();

        int node = front.first;
        int dist = front.second;

        if (node == id2) return dist;

        for (auto &edge : g.getNeighbors(node)) {
            if (!visited.count(edge.to)) {
                visited.insert(edge.to);
                q.push({edge.to, dist + 1});
            }
        }
    }

    return -1;
}

// Mutual Friends
std::vector<int> mutualFriends(const Graph& g, int id1, int id2) {
    std::vector<int> result;
    std::unordered_set<int> set1;

    for (auto &e : g.getNeighbors(id1))
        set1.insert(e.to);

    for (auto &e : g.getNeighbors(id2))
        if (set1.count(e.to))
            result.push_back(e.to);

    return result;
}

// Most Connected User
int mostConnectedUser(const Graph& g) {
    int maxUser = -1;
    int maxConnections = -1;

    for (int id : g.getAllUserIds()) {
        int size = g.getNeighbors(id).size();
        if (size > maxConnections) {
            maxConnections = size;
            maxUser = id;
        }
    }

    return maxUser;
}

// Graph Connected or Not
bool isGraphConnected(const Graph& g) {
    auto users = g.getAllUserIds();
    if (users.empty()) return true;

    std::queue<int> q;
    std::unordered_set<int> visited;

    q.push(users[0]);
    visited.insert(users[0]);

    while (!q.empty()) {
        int node = q.front();
        q.pop();

        for (auto &edge : g.getNeighbors(node)) {
            if (!visited.count(edge.to)) {
                visited.insert(edge.to);
                q.push(edge.to);
            }
        }
    }

    return visited.size() == users.size();
}

// Reachable Users
std::vector<int> reachableUsers(const Graph& g, int id) {
    std::vector<int> result;
    std::queue<int> q;
    std::unordered_set<int> visited;

    q.push(id);
    visited.insert(id);

    while (!q.empty()) {
        int node = q.front();
        q.pop();

        result.push_back(node);

        for (auto &edge : g.getNeighbors(node)) {
            if (!visited.count(edge.to)) {
                visited.insert(edge.to);
                q.push(edge.to);
            }
        }
    }

    return result;
}

// Average Degree
double averageDegree(const Graph& g) {
    int total = 0;
    auto users = g.getAllUserIds();

    for (int id : users) {
        total += g.getNeighbors(id).size();
    }

    if (users.empty()) return 0;

    return (double) total / users.size();
}


int main() {
    Graph g;

    // sample graph
    g.adj[1] = {{2,1}, {3,1}};
    g.adj[2] = {{1,1}, {4,1}};
    g.adj[3] = {{1,1}};
    g.adj[4] = {{2,1}};

    std::cout << "Degree of Separation (1 → 4): "
              << degreeOfSeparation(g, 1, 4) << std::endl;

    std::cout << "Most Connected User: "
              << mostConnectedUser(g) << std::endl;

    return 0;
}