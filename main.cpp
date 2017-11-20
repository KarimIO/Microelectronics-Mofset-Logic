#include "graph.hpp"
#include "graph_converter.hpp"
#include <iostream>
#include <string>
#include <map>

std::string ConvertToMofset(Node *input) {
	Node *pun = input->DeMorgan();
	std::cout << "PUN: " << *pun << std::endl;
	Node *pdn = pun->Invert();
	std::cout << "PDN: " << *pdn << std::endl;


	unsigned int transistor_count_inverters = 0;

	unsigned int transistor_count = 0;
	std::map<std::string, unsigned int> inverters;
	std::string output;
	output += "==============\nPUN Network: \n" + pun->Mosfet("Vdd", "y", PUN, inverters, transistor_count);
	output += "==============\nPDN Network: \n" + pdn->Mosfet("y", "gnd", PDN, inverters, transistor_count);

	if (inverters.size() > 0) {
		std::string output2 = "Inverters:\n";
		for (auto &inverter : inverters) {
			output2 += std::string("The inverter circuit for ") + inverter.first + " is used " + std::to_string(inverter.second) + " times: \n";
			output2 += std::string("\tMi") + std::to_string(transistor_count_inverters++) + " wire_inv_" + inverter.first + " " + inverter.first + " Vdd Vdd PMOS\n";
			output2 += std::string("\tMi") + std::to_string(transistor_count_inverters++) + " wire_inv_" + inverter.first + " " + inverter.first + " gnd gnd NMOS\n";
		}
		output = output2 + output;
	}

	output = output + "Total Number of transistors: " + std::to_string(transistor_count_inverters + transistor_count);

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