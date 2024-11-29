#include "Graph.hpp"

Graph::Graph(const std::vector<Node> nodes) : _nodes(nodes)
{
}

Graph::~Graph()
{
}

void Graph::addNode(const Node &node)
{
    _nodes.push_back(node);
}

std::vector<Node> Graph::findPath(const Node &from, const Node &to) const
{
    std::vector<Node> vec;
    // Implement Djikstra's algorithm
    return vec;
}