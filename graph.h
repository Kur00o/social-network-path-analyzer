#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

// ── User ────────────────────────────────────────────────────────────────────
class User {
private:
    int         id;
    std::string name;
    int         age;
public:
    User();
    User(int id, const std::string& name, int age);

    int         getId()   const;
    std::string getName() const;
    int         getAge()  const;
    void        display() const;
};

// ── Edge ────────────────────────────────────────────────────────────────────
class Edge {
private:
    int    from;
    int    to;
    double weight;
public:
    Edge();
    Edge(int from, int to, double weight = 1.0);

    int    getFrom()   const;
    int    getTo()     const;
    double getWeight() const;
    void   display()   const;
};

// ── Graph ───────────────────────────────────────────────────────────────────
class Graph {
private:
    std::unordered_map<int, User>              users;
    std::unordered_map<int, std::vector<Edge>> adjList;
    std::vector<Edge>                          edges;
    bool                                       directed;

public:
    Graph();
    Graph(bool directed);

    void addUser(const User& user);
    void addEdge(int from, int to, double weight = 1.0);
    void removeUser(int userId);
    void removeEdge(int from, int to);

    User              getUser(int userId)  const;
    std::vector<Edge> getNeighbors(int userId) const;
    std::unordered_map<int, User> getAllUsers() const;
    std::vector<Edge> getAllEdges() const;

    bool hasUser(int userId) const;
    bool hasEdge(int from, int to) const;
    int  getUserCount() const;
    int  getEdgeCount()  const;

    void display() const;
};
