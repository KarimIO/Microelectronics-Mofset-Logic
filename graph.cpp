#include "graph.hpp"
#include <iostream>

InNode::InNode(char name) : a_(name) {}
NotNode::NotNode(Node *a) : a_(a) {}
AndNode::AndNode(Node *a, Node *b) : a_(a), b_(b) {}
OrNode::OrNode(Node *a, Node *b) : a_(a), b_(b) {}

std::string InNode::Traverse() {
    return std::string(&a_);
}

std::string NotNode::Traverse() {
    return a_->Traverse() + "'";
}

std::string AndNode::Traverse() {
    return "(" + a_->Traverse() + "&" + b_->Traverse() + ")";
}

std::string OrNode::Traverse() {
    return "(" + a_->Traverse() + "|" + b_->Traverse() + ")";
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