#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include "interface.h"
#include "shortest_path.h"
#include "analysis.h"

// ── Constructor ──────────────────────────────────────────────────────────────

Interface::Interface() : graph(false) {} // undirected graph

// ── Helpers ──────────────────────────────────────────────────────────────────

int Interface::getIntInput(const std::string& prompt) {
    int val;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val) break;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  Invalid input. Please enter a number.\n";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return val;
}

double Interface::getDoubleInput(const std::string& prompt) {
    double val;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val) break;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  Invalid input.\n";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return val;
}

void Interface::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Interface::printHeader() {
    std::cout << "\n================================================\n";
    std::cout << "   Social Network Shortest Path Analyzer\n";
    std::cout << "================================================\n";
}

// ── Load from file ───────────────────────────────────────────────────────────

void Interface::menuLoadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Could not open file: " << filename << "\n";
        return;
    }

    std::string line;
    std::string section;
    int loaded = 0;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        if (line == "[USERS]")   { section = "users";   continue; }
        if (line == "[EDGES]")   { section = "edges";   continue; }

        std::istringstream ss(line);
        if (section == "users") {
            int id, age; std::string name;
            if (ss >> id >> name >> age) {
                graph.addUser(User(id, name, age));
                ++loaded;
            }
        } else if (section == "edges") {
            int from, to; double weight;
            if (ss >> from >> to >> weight) {
                graph.addEdge(from, to, weight);
                ++loaded;
            }
        }
    }
    std::cout << "Loaded " << loaded << " entries from " << filename << ".\n";
}

// ── Default sample data ──────────────────────────────────────────────────────

void Interface::loadDefaultData() {
    graph.addUser(User(1, "Alice",   22));
    graph.addUser(User(2, "Bob",     25));
    graph.addUser(User(3, "Charlie", 23));
    graph.addUser(User(4, "Diana",   28));
    graph.addUser(User(5, "Eve",     21));
    graph.addUser(User(6, "Frank",   30));
    graph.addUser(User(7, "Grace",   24));

    graph.addEdge(1, 2, 1.0);
    graph.addEdge(1, 3, 2.0);
    graph.addEdge(2, 4, 1.5);
    graph.addEdge(3, 4, 1.0);
    graph.addEdge(4, 5, 2.0);
    graph.addEdge(5, 6, 1.0);
    graph.addEdge(6, 7, 1.5);
    graph.addEdge(2, 7, 3.0);
    graph.addEdge(3, 5, 2.5);

    std::cout << "Sample data loaded (7 users, 9 connections).\n";
}

// ── Sub-menus ────────────────────────────────────────────────────────────────

void Interface::menuAddUser() {
    int id  = getIntInput("  Enter User ID   : ");
    std::cout << "  Enter Name      : ";
    std::string name; std::getline(std::cin, name);
    int age = getIntInput("  Enter Age       : ");
    graph.addUser(User(id, name, age));
    std::cout << "  User added.\n";
}

void Interface::menuAddEdge() {
    int    from   = getIntInput("  From User ID    : ");
    int    to     = getIntInput("  To   User ID    : ");
    double weight = getDoubleInput("  Weight (default 1): ");
    graph.addEdge(from, to, weight);
    std::cout << "  Edge added.\n";
}

void Interface::menuRemoveUser() {
    int id = getIntInput("  User ID to remove: ");
    graph.removeUser(id);
    std::cout << "  User removed.\n";
}

void Interface::menuRemoveEdge() {
    int from = getIntInput("  From User ID: ");
    int to   = getIntInput("  To   User ID: ");
    graph.removeEdge(from, to);
    std::cout << "  Edge removed.\n";
}

void Interface::menuShortestPath() {
    std::cout << "\n  -- Shortest Path --\n";
    int src = getIntInput("  Source User ID     : ");
    int dst = getIntInput("  Destination User ID: ");

    std::cout << "\n  [1] BFS (unweighted / hop count)\n";
    std::cout << "  [2] Dijkstra (weighted)\n";
    int choice = getIntInput("  Choose algorithm   : ");

    PathResult result;
    if (choice == 1)
        result = ShortestPath::bfsShortestPath(graph, src, dst);
    else
        result = ShortestPath::dijkstraShortestPath(graph, src, dst);

    std::cout << "\n";
    result.display(graph);
}

void Interface::menuAnalysis() {
    std::cout << "\n  -- Network Analysis --\n";
    std::cout << "  [1] Degree of separation between two users\n";
    std::cout << "  [2] Mutual friends between two users\n";
    std::cout << "  [3] Most connected user\n";
    std::cout << "  [4] Is graph connected?\n";
    std::cout << "  [5] Reachable users from a user\n";
    std::cout << "  [6] Influence score of a user\n";
    std::cout << "  [7] Full network summary\n";
    int choice = getIntInput("  Choice: ");

    switch (choice) {
        case 1: {
            int u1 = getIntInput("  User 1 ID: ");
            int u2 = getIntInput("  User 2 ID: ");
            int deg = Analysis::degreeOfSeparation(graph, u1, u2);
            if (deg == -1)
                std::cout << "  No connection found.\n";
            else
                std::cout << "  Degrees of separation: " << deg << "\n";
            break;
        }
        case 2: {
            int u1 = getIntInput("  User 1 ID: ");
            int u2 = getIntInput("  User 2 ID: ");
            auto mutual = Analysis::mutualFriends(graph, u1, u2);
            if (mutual.empty()) {
                std::cout << "  No mutual friends.\n";
            } else {
                std::cout << "  Mutual friends: ";
                for (int id : mutual)
                    std::cout << graph.getUser(id).getName() << " ";
                std::cout << "\n";
            }
            break;
        }
        case 3: {
            User top = Analysis::mostConnectedUser(graph);
            std::cout << "  Most connected: " << top.getName()
                      << " (ID " << top.getId() << ", "
                      << Analysis::degree(graph, top.getId()) << " connections)\n";
            break;
        }
        case 4:
            std::cout << "  Graph is "
                      << (Analysis::isGraphConnected(graph) ? "connected.\n" : "NOT connected.\n");
            break;
        case 5: {
            int uid = getIntInput("  User ID: ");
            auto reachable = Analysis::reachableUsers(graph, uid);
            std::cout << "  Reachable users (" << reachable.size() << "): ";
            for (int id : reachable)
                std::cout << graph.getUser(id).getName() << " ";
            std::cout << "\n";
            break;
        }
        case 6: {
            int uid = getIntInput("  User ID: ");
            std::cout << "  Influence score: "
                      << Analysis::influenceScore(graph, uid) << "\n";
            break;
        }
        case 7:
            Analysis::networkSummary(graph);
            break;
        default:
            std::cout << "  Invalid choice.\n";
    }
}

void Interface::menuDisplayGraph() {
    graph.display();
}

// ── Main run loop ────────────────────────────────────────────────────────────

void Interface::run() {
    printHeader();
    std::cout << "\nLoad sample data? (y/n): ";
    char ch; std::cin >> ch;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (ch == 'y' || ch == 'Y') loadDefaultData();

    bool running = true;
    while (running) {
        printHeader();
        std::cout << "  [1] Add User\n";
        std::cout << "  [2] Add Connection\n";
        std::cout << "  [3] Remove User\n";
        std::cout << "  [4] Remove Connection\n";
        std::cout << "  [5] Find Shortest Path\n";
        std::cout << "  [6] Network Analysis\n";
        std::cout << "  [7] Display Graph\n";
        std::cout << "  [8] Load from file (data.txt)\n";
        std::cout << "  [0] Exit\n";
        std::cout << "------------------------------------------------\n";

        int choice = getIntInput("  Choice: ");

        switch (choice) {
            case 1: menuAddUser();             break;
            case 2: menuAddEdge();             break;
            case 3: menuRemoveUser();          break;
            case 4: menuRemoveEdge();          break;
            case 5: menuShortestPath();        break;
            case 6: menuAnalysis();            break;
            case 7: menuDisplayGraph();        break;
            case 8: menuLoadFromFile("data.txt"); break;
            case 0: running = false;           break;
            default: std::cout << "  Invalid choice.\n";
        }

        if (running) {
            std::cout << "\nPress Enter to continue...";
            std::cin.get();
        }
    }
    std::cout << "\nGoodbye!\n";
}
