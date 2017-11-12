#include "graph_converter.hpp"
#include <iostream>

// Not final. a&b|c'
Node *GraphConverter::ConvertToNodes(std::string input) {
    for (char &c : input) {
        if (c == '\'') {
            Node *n = nodestack.top();
            nodestack.pop();
            nodestack.push(new NotNode(n)); 
        }
        else if (c == '|' || c == '&') {
            processOperator(c);
        }
        else if (c == '(') {
            opstack.push('(');
        }
        else if (c == ')') {
            parseRightParenthesis();
        }
        else {
            nodestack.push(new InNode(c));
        }
    }

    while (!opstack.empty()) {
        doBinary( opstack.top() );
        opstack.pop();
    }

    if (nodestack.size() != 1) {
        std::cerr << "Runtime Error! Incorrectly formed binary logic.";
    }

    return nodestack.top();
}

void GraphConverter::parseRightParenthesis() {
    while (!opstack.empty() && opstack.top() != '(' ) {
        doBinary( opstack.top() );
        opstack.pop();
    }

    opstack.pop();
}

void GraphConverter::doBinary(char c) {
    Node *r = nodestack.top();
    nodestack.pop();

    Node *l = nodestack.top();
    nodestack.pop();

    if (c == '&') {
        nodestack.push(new AndNode(l, r));
    }
    else if (c == '|') {
        nodestack.push(new OrNode(l, r));
    }
    else {
        std::cerr << "Nodestack dobinary issue\n";
    }
}

void GraphConverter::processOperator(char c) {
    while (!opstack.empty() && opstack.top() != '(' )  {
        doBinary( opstack.top() );
        opstack.pop();
    }

    // lastly push the operator passed onto the operatorStack
    opstack.push(c);
}