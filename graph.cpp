#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <stdexcept>
#include "graph.h"

// ── User ────────────────────────────────────────────────────────────────────

User::User() : id(0), name(""), age(0) {}

User::User(int id, const std::string& name, int age)
    : id(id), name(name), age(age) {}

int         User::getId()   const { return id; }
std::string User::getName() const { return name; }
int         User::getAge()  const { return age; }

void User::display() const {
    std::cout << "[User " << id << "] " << name << " (age " << age << ")\n";
}

// ── Edge ────────────────────────────────────────────────────────────────────

Edge::Edge() : from(0), to(0), weight(1.0) {}

Edge::Edge(int from, int to, double weight)
    : from(from), to(to), weight(weight) {}

int    Edge::getFrom()   const { return from; }
int    Edge::getTo()     const { return to; }
double Edge::getWeight() const { return weight; }

void Edge::display() const {
    std::cout << from << " --(" << weight << ")--> " << to << "\n";
}

// ── Graph ───────────────────────────────────────────────────────────────────

Graph::Graph() : directed(false) {}

Graph::Graph(bool directed) : directed(directed) {}

// Add a user node
void Graph::addUser(const User& user) {
    int id = user.getId();
    if (users.count(id)) {
        std::cout << "User " << id << " already exists.\n";
        return;
    }
    users[id] = user;
    adjList[id]; // initialise empty adjacency list
}

// Add a weighted edge (undirected by default)
void Graph::addEdge(int from, int to, double weight) {
    if (!users.count(from) || !users.count(to)) {
        std::cerr << "Error: one or both users not found.\n";
        return;
    }
    adjList[from].push_back(Edge(from, to, weight));
    if (!directed)
        adjList[to].push_back(Edge(to, from, weight));

    edges.push_back(Edge(from, to, weight));
}

// Remove a user and all associated edges
void Graph::removeUser(int userId) {
    if (!users.count(userId)) {
        std::cerr << "User " << userId << " not found.\n";
        return;
    }
    users.erase(userId);
    adjList.erase(userId);

    // Remove edges pointing to this user
    for (auto& [uid, edgeList] : adjList) {
        edgeList.erase(
            std::remove_if(edgeList.begin(), edgeList.end(),
                [userId](const Edge& e){ return e.getTo() == userId; }),
            edgeList.end()
        );
    }
    edges.erase(
        std::remove_if(edges.begin(), edges.end(),
            [userId](const Edge& e){
                return e.getFrom() == userId || e.getTo() == userId;
            }),
        edges.end()
    );
}

// Remove a specific edge
void Graph::removeEdge(int from, int to) {
    auto removeFrom = [&](int src, int dst) {
        auto& list = adjList[src];
        list.erase(
            std::remove_if(list.begin(), list.end(),
                [dst](const Edge& e){ return e.getTo() == dst; }),
            list.end()
        );
    };
    removeFrom(from, to);
    if (!directed) removeFrom(to, from);
}

// Getters
User Graph::getUser(int userId) const {
    auto it = users.find(userId);
    if (it == users.end())
        throw std::runtime_error("User not found: " + std::to_string(userId));
    return it->second;
}

std::vector<Edge> Graph::getNeighbors(int userId) const {
    auto it = adjList.find(userId);
    if (it == adjList.end()) return {};
    return it->second;
}

std::unordered_map<int, User> Graph::getAllUsers() const {
    return users;
}

std::vector<Edge> Graph::getAllEdges() const {
    return edges;
}

bool Graph::hasUser(int userId) const {
    return users.count(userId) > 0;
}

bool Graph::hasEdge(int from, int to) const {
    auto it = adjList.find(from);
    if (it == adjList.end()) return false;
    for (const auto& e : it->second)
        if (e.getTo() == to) return true;
    return false;
}

int Graph::getUserCount() const { return (int)users.size(); }
int Graph::getEdgeCount()  const { return (int)edges.size(); }

// Display the entire graph
void Graph::display() const {
    std::cout << "\n=== Social Network Graph ===\n";
    std::cout << "Users: " << users.size()
              << "  |  Connections: " << edges.size() << "\n";
    std::cout << "----------------------------\n";
    for (const auto& [uid, user] : users) {
        user.display();
        const auto& neighbors = adjList.at(uid);
        if (neighbors.empty()) {
            std::cout << "   (no connections)\n";
        } else {
            for (const auto& e : neighbors) {
                std::cout << "   -> "
                          << users.at(e.getTo()).getName()
                          << " (weight: " << e.getWeight() << ")\n";
            }
        }
    }
    std::cout << "============================\n";
}
