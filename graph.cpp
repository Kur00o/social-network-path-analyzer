#include "graph.h"

bool Graph::userExists(int id)
{
    return users.find(id) != users.end();
}

void Graph::addUser(int id, string name, int age)
{
    if (userExists(id))
    {
        cout << "User already exists\n";
        return;
    }

    User u;
    u.id = id;
    u.name = name;
    u.age = age;

    users[id] = u;
    adjList[id] = vector<Edge>();
}

void Graph::addFriendship(int id1, int id2, int weight)
{
    if (!userExists(id1) || !userExists(id2))
    {
        cout << "User not found\n";
        return;
    }
    Edge e1;
    e1.to = id2;
    e1.weight = weight;

    Edge e2;
    e2.to = id1;
    e2.weight = weight;

    adjList[id1].push_back(e1);
    adjList[id2].push_back(e2);
}

void Graph::displayGraph()
{
    cout << "\nSocial Network Graph\n";

    for (auto u : users)
    {
        cout << u.second.name << " -> ";

        for (auto e : adjList[u.first])
        {
            cout << users[e.to].name << " ";
        }

        cout << endl;
    }
}