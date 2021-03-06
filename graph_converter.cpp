#include "graph_converter.hpp"
#include <iostream>

// Not final. a&b|c'
Node *GraphConverter::ConvertToNodes(std::string input) {
    for (int i = 0; i < input.size(); i++) {
        char c = input[i];
        if (c == '\'') {
            Node *n = nodestack.top();
            nodestack.pop();
            nodestack.push(new NotNode(n)); 
		}
		else if (c == '&') {
			processOperator(c);
		}
		else if (c == '|') {
			processOperator2(c);
		}
        else if (c == '(') {
            opstack.push('(');
        }
        else if (c == ')') {
            parseRightParenthesis();
        }
        else {
            std::string variable;
            for (;i < input.size(); i++) {
                if (input[i] == ')' || input[i] == '(' || input[i] == '|'
                    || input[i] == '\'' || input[i] == '&') {
                    i--;
                    break;
                }

                variable.push_back(input[i]);
            }
            nodestack.push(new InNode(variable));
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
	while (!opstack.empty() && opstack.top() != '(' && opstack.top() != '|')  {
		doBinary(opstack.top());
		opstack.pop();
	}

	// lastly push the operator passed onto the operatorStack
	opstack.push(c);
}

void GraphConverter::processOperator2(char c) {
	while (!opstack.empty() && opstack.top() != '(')  {
		doBinary(opstack.top());
		opstack.pop();
	}

	// lastly push the operator passed onto the operatorStack
	opstack.push(c);
}