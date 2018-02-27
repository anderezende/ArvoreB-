#include "Node.hpp"

Node::Node(int aOrder) : fOrder{aOrder}, fParent{nullptr} {}

Node::Node(int aOrder, Node* aParent) : fOrder{aOrder}, fParent{aParent} {}

Node::~Node() {}

int Node::order() const
{
    return fOrder;
}

Node* Node::parent() const
{
    return fParent;
}

void Node::setParent(Node* aParent)
{
    fParent = aParent;
}

bool Node::isRoot() const
{
    return !fParent;
}
