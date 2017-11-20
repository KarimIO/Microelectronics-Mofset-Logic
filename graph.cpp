#include "graph.hpp"
#include <iostream>
#include <string>

InNode::InNode(std::string name) : name_(name) {}
NotNode::NotNode(Node *a) : a_(a) {}
AndNode::AndNode(Node *a, Node *b) : a_(a), b_(b) {}
OrNode::OrNode(Node *a, Node *b) : a_(a), b_(b) {}

NodeType InNode::GetType() const { return INNODE; }
NodeType NotNode::GetType() const { return NOTNODE; }
NodeType AndNode::GetType() const { return ANDNODE; }
NodeType OrNode::GetType() const { return ORNODE; }

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

Node *InNode::DeMorgan() const {
	return new InNode(name_);
}

Node *NotNode::DeMorgan() const {
    if (a_->GetType() == INNODE) {
        return new NotNode(new InNode(a_->Traverse()));
    }
    else {
    	return a_->Invert()->DeMorgan();
    }
}

Node *OrNode::DeMorgan() const {
	return new OrNode(a_->DeMorgan(), b_->DeMorgan());
}

Node *AndNode::DeMorgan() const {
	return new AndNode(a_->DeMorgan(), b_->DeMorgan());
}

unsigned int wire_count = 0;

std::string InNode::Mosfet(std::string up, std::string down, Network network, std::map<std::string, unsigned int> &inverters, unsigned int &transistor_count) const {
	if (network == PUN) {
		// PUN
		inverters[name_]++;
		return std::string("\tM") + std::to_string(transistor_count++) + " " + down + " wire_inv_" + name_ + " " + up + " " + up + " PMOS\n";
	}
	else {
		// PDN
		return std::string("\tM") + std::to_string(transistor_count++) + " " + up + " " + name_ + " " + down + " " + down + " NMOS\n";
	}
}

std::string NotNode::Mosfet(std::string up, std::string down, Network network, std::map<std::string, unsigned int> &inverters, unsigned int &transistor_count) const {
	// Note: All children should be innodes at this stage, due to DeMorgan
	std::string child_name = a_->Traverse();
	
	if (network == PUN) {
		// PUN
		return std::string("\tM") + std::to_string(transistor_count++) + " " + down + " " + child_name + " " + up + " " + up + " PMOS\n";
	}
	else {
		// PDN
		inverters[child_name]++;
		return std::string("\tM") + std::to_string(transistor_count++) + " " + up + " wire_inv_" + child_name + " " + down + " " + down + " NMOS\n";
	}
}

std::string AndNode::Mosfet(std::string up, std::string down, Network network, std::map<std::string, unsigned int> &inverters, unsigned int &transistor_count) const {
	std::string wire_name = std::string("w_") + std::to_string(wire_count++);
	std::string out = a_->Mosfet(up, wire_name, network, inverters, transistor_count);
	out += b_->Mosfet(wire_name, down, network, inverters, transistor_count);
	return out;
}

std::string OrNode::Mosfet(std::string up, std::string down, Network network, std::map<std::string, unsigned int> &inverters, unsigned int &transistor_count) const {
	std::string out = a_->Mosfet(up, down, network, inverters, transistor_count);
	out += b_->Mosfet(up, down, network, inverters, transistor_count);
	return out;
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