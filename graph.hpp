#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>

class Node {
public:
    virtual std::string Traverse() = 0;
    virtual ~Node() {};
};

class InNode : public Node {
public:
    InNode(char name);
    virtual std::string Traverse();
    virtual ~InNode();
private:
    char a_;
};

class NotNode : public Node {
public:
    NotNode(Node *a);
    virtual std::string Traverse();
    virtual ~NotNode();
private:
    Node *a_;
};

class OrNode : public Node {
public:
    OrNode(Node *a, Node *b);
    virtual std::string Traverse();
    virtual ~OrNode();
private:
    Node *a_;
    Node *b_;
};

class AndNode : public Node {
public:
    AndNode(Node *a, Node *b);
    virtual std::string Traverse();
    virtual ~AndNode();
private:
    Node *a_;
    Node *b_;
};

#endif