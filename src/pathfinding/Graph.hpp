#pragma once

#include "Node.hpp"
#include "Link.hpp"
#include <queue>

class Graph
{
private:
    unsigned long _currentId = 0;

    std::vector<Node> _nodes;

public:
    Graph(std::vector<Node> nodes = std::vector<Node>());
    ~Graph();

    void addNode(Vector3 pos);
    void removeNode(Node* node);

    std::vector<Node> &getNodes();

    int nodeCount();

    std::vector<Node*> findPath(const Node &from, const Node &to, const char vehicleType) const;
};