#include "graph.hpp"
#include <iostream>
#include <string>

InNode::InNode(std::string name) : name_(name) {}
NotNode::NotNode(Node *a) : a_(a) {}
AndNode::AndNode(Node *a, Node *b) : a_(a), b_(b) {}
OrNode::OrNode(Node *a, Node *b) : a_(a), b_(b) {}

std::string InNode::Traverse() const {
	return name_;
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

Node *InNode::Invert() const {
	return new NotNode(new InNode(name_));
}

Node *NotNode::Invert() const {
	return new InNode(a_->Traverse());
}

Node *OrNode::Invert() const {
	return new AndNode(a_->Invert(), b_->Invert());

}

Node *AndNode::Invert() const {
	return new OrNode(a_->Invert(), b_->Invert());
}

unsigned int wire_count, transistor_count;

std::string InNode::Mosfet(std::string up, std::string down, Network network) const {
	if (network == PUN) {
		// PUN
		std::string wire_name = "w_" + (wire_count++);
		std::string output = std::string("M") + std::to_string(transistor_count++) + " " + Traverse() + " Vdd " + wire_name + " " + wire_name + " PMOS\n";
		output += std::string("M") + std::to_string(transistor_count++) + " " + wire_name + " " + up + " " + down + " " + down + " PMOS\n";
		output += std::string("M") + std::to_string(transistor_count++) + " " + Traverse() + " " + wire_name + " gnd gnd NMOS\n";
		return output;
	}
	else {
		// PDN
		return std::string("M") + std::to_string(transistor_count++) + " " + Traverse() + " " + up + " " + down + " " + down + " NMOS\n";
	}
}

std::string NotNode::Mosfet(std::string up, std::string down, Network network) const {
	std::string child_name = a_->Traverse();
	if (network == PUN) {
		// PUN
		return std::string("M") + std::to_string(transistor_count++) + " " + child_name + " " + up + " " + down + down + " PMOS\n";
	}
	else {
		// PDN
		std::string wire_name = "w_" + (wire_count++);
		std::string output = std::string("M") + std::to_string(transistor_count++) + " " + child_name + " Vdd " + wire_name + " " + wire_name + " PMOS\n";
		output += std::string("M") + std::to_string(transistor_count++) + " " + wire_name + " " + up + " " + down + " " + down + " NMOS\n";
		output += std::string("M") + std::to_string(transistor_count++) + " " + child_name + " " + wire_name + " gnd gnd NMOS\n";
		return output;
	}
}

std::string AndNode::Mosfet(std::string up, std::string down, Network network) const {
	std::string wire_name = "w_" + (wire_count++);
	return a_->Mosfet(up, wire_name, network) + b_->Mosfet(up, down, network);
}

std::string OrNode::Mosfet(std::string up, std::string down, Network network) const {
	return a_->Mosfet(up, down, network) + b_->Mosfet(up, down, network);
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