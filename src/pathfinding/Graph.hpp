#pragma once

#include "Node.hpp"
#include "Link.hpp"
#include <queue>

class Graph
{
private:
    std::vector<Node> _nodes;

public:
    Graph(const std::vector<Node> nodes = std::vector<Node>());
    ~Graph();

    void addNode(const Node &node);
    std::vector<Node> findPath(const Node &from, const Node &to) const;
};