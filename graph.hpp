#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <string>
#include <map>

enum Network {
	PUN = 0,
	PDN
};

enum NodeType {
	INNODE = 0,
	NOTNODE,
	ORNODE,
	ANDNODE
};

class Node {
public:
	virtual NodeType GetType() const = 0;
	virtual std::string Traverse() const = 0;
	virtual Node *Invert() const = 0;
	virtual Node *DeMorgan() const = 0;
	virtual std::string Mosfet(std::string up, std::string down, Network network, std::map<std::string, unsigned int> &inverters, unsigned int &transistors_count) const = 0;
    friend std::ostream& operator<<(std::ostream& os, const Node& dt); 
    virtual ~Node() {};
};

class InNode : public Node {
public:
    InNode(std::string name);
	virtual inline NodeType GetType() const;
	virtual std::string Traverse() const;
	virtual Node *Invert() const;
	virtual Node *DeMorgan() const;
	virtual std::string Mosfet(std::string up, std::string down, Network network, std::map<std::string, unsigned int> &inverters, unsigned int &transistors_count) const;
    virtual ~InNode();
private:
    std::string name_;
};

class NotNode : public Node {
public:
    NotNode(Node *a);
	virtual inline NodeType GetType() const;
	virtual std::string Traverse() const;
	virtual Node *Invert() const;
	virtual Node *DeMorgan() const;
	virtual std::string Mosfet(std::string up, std::string down, Network network, std::map<std::string, unsigned int> &inverters, unsigned int &transistors_count) const;
    virtual ~NotNode();
private:
    Node *a_;
};

class OrNode : public Node {
public:
    OrNode(Node *a, Node *b);
	virtual inline NodeType GetType() const;
	virtual std::string Traverse() const;
	virtual Node *Invert() const;
	virtual Node *DeMorgan() const;
	virtual std::string Mosfet(std::string up, std::string down, Network network, std::map<std::string, unsigned int> &inverters, unsigned int &transistors_count) const;
    virtual ~OrNode();
private:
    Node *a_;
    Node *b_;
};

class AndNode : public Node {
public:
    AndNode(Node *a, Node *b);
	virtual inline NodeType GetType() const;
	virtual std::string Traverse() const;
	virtual Node *Invert() const;
	virtual Node *DeMorgan() const;
	virtual std::string Mosfet(std::string up, std::string down, Network network, std::map<std::string, unsigned int> &inverters, unsigned int &transistors_count) const;
    virtual ~AndNode();
private:
    Node *a_;
    Node *b_;
};

#endif