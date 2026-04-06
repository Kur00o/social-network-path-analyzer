#pragma once
#include <vector>
#include <string>
#include "graph.h"

// ── PathResult ───────────────────────────────────────────────────────────────
struct PathResult {
    std::vector<int> path;       // sequence of user IDs
    double           totalCost;
    bool             found;

    PathResult() : totalCost(0.0), found(false) {}
    void display(const Graph& g) const;
};

// ── ShortestPath ─────────────────────────────────────────────────────────────
class ShortestPath {
public:
    // Unweighted shortest path (hop count)
    static PathResult bfsShortestPath(const Graph& g, int srcId, int dstId);

    // Weighted shortest path (Dijkstra)
    static PathResult dijkstraShortestPath(const Graph& g, int srcId, int dstId);
};
