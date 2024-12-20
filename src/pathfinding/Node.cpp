#include "Node.hpp"

Node::Node(const float &x, const float &z) : _pos((Vector3){x, 0, z}), _col(YELLOW)
{
}

Node::Node(const Vector3 &pos, bool start, bool end, Util::VehicleType allowedVehicles, Color color) : _pos(pos), _start(start), _end(end), _col(color), _allowedVehicles(allowedVehicles)
{
    _pos.y = 0;
}

Node::~Node()
{
}

void Node::link(Node *node)
{
    if (node != this && !isLinked(node))
        _links.insert(node);
}

bool Node::unlink(Node *node)
{
    if (_links.contains(node))
    {
        _links.erase(node);
        return true;
    }
    return false;
}

bool Node::isLinked(Node *node) const
{
    return _links.contains(node);
}

std::set<Node *> Node::getAllLinks() const
{
    return _links;
}

bool Node::isBlocked() const
{
    return false;
}

bool Node::allowsVehicle(Util::VehicleType vehicleType) const
{
    return (_allowedVehicles & vehicleType) != Util::VehicleType::None;
}

Color Node::getColor()
{
    return _col;
}

bool Node::isStart()
{
    return _start;
}

bool Node::isEnd()
{
    return _end;
}

Vector3 Node::getPos() const
{
    return _pos;
}