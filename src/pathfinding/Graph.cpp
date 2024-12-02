#include "Graph.hpp"

Graph::Graph(const std::vector<Node> nodes) : _nodes(nodes)
{
}

Graph::~Graph()
{
}

void Graph::addNode(Vector3 pos)
{
    // Inserts the new node, and incremets the current id
    _nodes.push_back(Node(pos, _currentId++));
}

/// @brief Removes a node from the graph, along with all of its links towards other nodes
/// @param node 
void Graph::removeNode(Node *node)
{
    std::vector<Node>::iterator found = _nodes.end();

    for (auto n = _nodes.begin(); n < _nodes.end(); n++)
    {
        if (&*n == node)
            found = n;
        else 
            n->unlink(*node);
    }

    if (found != _nodes.end())
        _nodes.erase(found);
}

std::vector<Node> &Graph::getNodes()
{
    return _nodes;
}

int Graph::nodeCount()
{
    return _nodes.size();
}

std::vector<Node *> Graph::findPath(const Node &from, const Node &to, const char vehicleType) const
{
    std::vector<Node *> vec;
    // Implement Djikstra's algorithm
    return vec;
}