#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>

enum Network {
	PUN = 0,
	PDN
};

class Node {
public:
	virtual std::string Traverse() const = 0;
	virtual Node *Invert() const = 0;
	virtual std::string Mosfet(std::string up, std::string down, Network network) const = 0;
    friend std::ostream& operator<<(std::ostream& os, const Node& dt); 
    virtual ~Node() {};
};

class InNode : public Node {
public:
    InNode(std::string name);
	virtual std::string Traverse() const;
	virtual Node *Invert() const;
	virtual std::string Mosfet(std::string up, std::string down, Network network) const;
    virtual ~InNode();
private:
    std::string name_;
};

class NotNode : public Node {
public:
    NotNode(Node *a);
	virtual std::string Traverse() const;
	virtual Node *Invert() const;
	virtual std::string Mosfet(std::string up, std::string down, Network network) const;
    virtual ~NotNode();
private:
    Node *a_;
};

class OrNode : public Node {
public:
    OrNode(Node *a, Node *b);
	virtual std::string Traverse() const;
	virtual Node *Invert() const;
	virtual std::string Mosfet(std::string up, std::string down, Network network) const;
    virtual ~OrNode();
private:
    Node *a_;
    Node *b_;
};

class AndNode : public Node {
public:
    AndNode(Node *a, Node *b);
	virtual std::string Traverse() const;
	virtual Node *Invert() const;
	virtual std::string Mosfet(std::string up, std::string down, Network network) const;
    virtual ~AndNode();
private:
    Node *a_;
    Node *b_;
};

#endif