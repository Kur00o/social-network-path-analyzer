#pragma once
#include "graph.h"

class Interface {
private:
    Graph graph;

    // Sub-menus
    void menuAddUser();
    void menuAddEdge();
    void menuRemoveUser();
    void menuRemoveEdge();
    void menuShortestPath();
    void menuAnalysis();
    void menuDisplayGraph();
    void menuLoadFromFile(const std::string& filename);

    // Helpers
    int  getIntInput(const std::string& prompt);
    double getDoubleInput(const std::string& prompt);
    void clearScreen();
    void printHeader();

public:
    Interface();
    void run();
    void loadDefaultData();
};
