#include "graph.hpp"
#include "graph_converter.hpp"
#include <iostream>
#include <string>

std::string ConvertToMofset(Node *input) {
	Node *pun = input->DeMorgan();
	std::cout << "PUN: " << *pun << std::endl;
	Node *pdn = pun->Invert();
	std::cout << "PDN: " << *pdn << std::endl;
	
	std::string output = "PUN Network: \n" + pun->Mosfet("Vdd", "y", PUN);
	output += "==============\nPDN Network: \n" + pdn->Mosfet("y", "gnd", PDN);

	delete pdn;
	delete pun;
	
	return output;
}

int main(int argc, const char *argv[]) {
	std::string input;
	std::cout << "Please insert boolean logic using |, &, and ': \n";
	std::cin >> input;
    GraphConverter conv;
    Node *n = conv.ConvertToNodes(input);
    std::cout << "Input: " << *n << std::endl;
    std::cout << ConvertToMofset(n) << std::endl;
    delete n;

#ifdef _MSC_VER
	system("pause");
#endif

    return 0;
}