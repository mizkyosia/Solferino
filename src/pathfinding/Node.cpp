#include "Node.hpp"

Node::Node(const float &x, const float &y) : _pos((Vector2){x, y})
{
}

Node::Node(const Vector2 &pos) : _pos(pos)
{
    Vector2 a(1, 2);
}

Node::~Node()
{
}

void Node::link(Node &node)
{
    if (!isLinked(node))
        _links.insert(&node);
}

bool Node::unlink(Node &node)
{
    if (_links.contains(&node))
    {
        _links.erase(&node);
        return true;
    }
    return false;
}

bool Node::isLinked(const Node &node) const
{
    for (auto i = _links.begin(); i != _links.end(); i++)
    {
        // If the two nodes share the same memory adress, they are one and the same
        if (*i == &node)
            return true;
    }
    return false;
}

std::optional<Node *> Node::getLink(const Node &node) const
{
    for (auto i = _links.begin(); i != _links.end(); i++)
    {
        // If the two nodes share the same memory adress, they are one and the same
        if (*i == &node)
            return std::optional(*i);
    }
    return std::nullopt;
}

std::set<Node *> Node::getAllLinks() const
{
    return _links;
}

bool Node::isBlocked() const
{
    return false;
}

Vector2 Node::getPos() const
{
    return _pos;
}