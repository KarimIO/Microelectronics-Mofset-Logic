#include "graph.hpp"
#include <iostream>
#include <string>

InNode::InNode(char name) : a_(name) {}
NotNode::NotNode(Node *a) : a_(a) {}
AndNode::AndNode(Node *a, Node *b) : a_(a), b_(b) {}
OrNode::OrNode(Node *a, Node *b) : a_(a), b_(b) {}

std::string InNode::Traverse() const {
	char c[2] = { a_, 0 };
	return c;
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

unsigned int wire_count, transistor_count;

std::string InNode::Mofset(std::string up, std::string down, Network network) const {
	if (network == PUN) {
		// PUN
		return std::string("M") + std::to_string(transistor_count) + " " + Traverse() + " " + up + " " + down + down + " PMOS";
	}
	else {
		// PDN
		return std::string("M") + std::to_string(transistor_count) + " " + Traverse() + " " + up + " " + down + down + " NMOS";
	}
}

std::string NotNode::Mofset(std::string up, std::string down, Network network) const {

}

std::string AndNode::Mofset(std::string up, std::string down, Network network) const {
	std::string wire_name = "w_" + (wire_count++);
	return a_->Mofset(up, wire_name, network) + b_->Mofset(up, down, network);
}

std::string OrNode::Mofset(std::string up, std::string down, Network network) const {
	return a_->Mofset(up, down, network) + b_->Mofset(up, down, network);
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