#pragma once

class Node;
class Link
{
private:
    Node* _end;
public:
    Link(Node *end);
    ~Link();

    Node* getNode() const;
};