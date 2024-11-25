#include "Link.hpp"

Link::Link(Node *end) : _end(end)
{
}

Link::~Link()
{
}

Node* Link::getNode() const {
    return _end;
}