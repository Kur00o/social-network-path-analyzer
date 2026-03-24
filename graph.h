#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

struct User
{
    int id;
    string name;
    int age;
};

struct Edge
{
    int to;
    int weight;
};

class Graph
{
private:
    unordered_map<int, User> users;
    unordered_map<int, vector<Edge>> adjList;

public:
    void addUser(int id, string name, int age);
    void addFriendship(int id1, int id2, int weight = 1);
    bool userExists(int id);
    void displayGraph();
};