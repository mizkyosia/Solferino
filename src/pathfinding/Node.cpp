#include "Node.hpp"

Node::Node(unsigned long id, const float &x, const float &z) : _pos((Vector3){x, 0, z}), _id(id)
{
}

Node::Node(const Vector3 &pos, unsigned long id) : _pos(pos), _id(id)
{
    _pos.y = 0;
}

Node::~Node()
{
}

void Node::link(Node &node)
{
    TraceLog(LOG_WARNING,"Inserting pointer to node : %x", &node);
    if (!isLinked(node))
        _links.insert(&node);
}

bool Node::unlink(Node &node)
{
    TraceLog(LOG_WARNING, "Unlinking node pointer for a node : %x", &node);
    if (_links.contains(&node))
    {
        TraceLog(LOG_WARNING, "UNLINKED NODE !!!!!!!");
        _links.erase(&node);
        return true;
    }
    return false;
}

bool Node::isLinked(Node &node) const
{
    for (auto i : _links)
    {
        // If the two nodes share the same memory adress, they are one and the same
        if (i == &node)
            return true;
    }
    return false;
}

std::optional<Node *> Node::getLink(Node &node) const
{
    for (auto i : _links)
    {
        // If the two nodes share the same memory adress, they are one and the same
        if (i == &node)
            return std::optional(i);
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

Color Node::getColor()
{
    return _col;
}

Vector3 Node::getPos() const
{
    return _pos;
}

bool Node::operator==(const Node &other) const
{
    return other._id == _id;
}
