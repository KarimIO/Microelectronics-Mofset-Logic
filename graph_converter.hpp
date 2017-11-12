#ifndef GRAPH_CONVERTER_H
#define GRAPH_CONVERTER_H

#include <stack>
#include "graph.hpp"

class GraphConverter {
public:
    void doBinary(char c);
    void processOperator(char c);
    void parseRightParenthesis();
    Node *ConvertToNodes(std::string input);
private:
    std::stack<char> opstack;
    std::stack<Node *> nodestack;
};

#endif