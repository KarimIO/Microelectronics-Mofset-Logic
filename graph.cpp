#include "graph.hpp"
#include <iostream>

InNode::InNode(char name) : a_(name) {}
NotNode::NotNode(Node *a) : a_(a) {}
AndNode::AndNode(Node *a, Node *b) : a_(a), b_(b) {}
OrNode::OrNode(Node *a, Node *b) : a_(a), b_(b) {}

std::string InNode::Traverse() const {
    return std::string(&a_);
}

std::string NotNode::Traverse() const {
    return a_->Traverse() + "'";
}

std::string AndNode::Traverse() const {
    return "(" + a_->Traverse() + "&" + b_->Traverse() + ")";
}

std::string OrNode::Traverse() const {
    return "(" + a_->Traverse() + "|" + b_->Traverse() + ")";
}

std::ostream& operator<<(std::ostream& os, const Node& dt) {
    os << dt.Traverse();
    return os;
}

InNode::~InNode() {}

NotNode::~NotNode() {
    delete a_;
}

AndNode::~AndNode() {
    delete a_;
    delete b_;
}

OrNode::~OrNode() {
    delete a_;
    delete b_;
}