#pragma once
#include <vector>
#include <unordered_map>
#include "graph.h"

class Analysis {
public:
    // Degree of separation between two users (hop count via BFS)
    static int degreeOfSeparation(const Graph& g, int userId1, int userId2);

    // Common friends between two users
    static std::vector<int> mutualFriends(const Graph& g, int userId1, int userId2);

    // User with the most connections
    static User mostConnectedUser(const Graph& g);

    // Check whether the entire graph is connected
    static bool isGraphConnected(const Graph& g);

    // Degree (number of direct connections) of a user
    static int degree(const Graph& g, int userId);

    // All users reachable from a given user (BFS)
    static std::vector<int> reachableUsers(const Graph& g, int userId);

    // Influence score: sum of weights of all outgoing edges
    static double influenceScore(const Graph& g, int userId);

    // Display a summary report for the whole network
    static void networkSummary(const Graph& g);
};
