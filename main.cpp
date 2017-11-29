#include "graph.hpp"
#include "graph_converter.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <map>

std::string ConvertToMofset(Node *input, float p, float n, float l, bool use_sizing) {
	Node *pun = input->DeMorgan();
	std::cout << "PUN: " << *pun << std::endl;
	Node *pdn = pun->Invert();
	std::cout << "PDN: " << *pdn << std::endl;


	unsigned int transistor_count_inverters = 0;

	unsigned int transistor_count = 0;
	std::map<std::string, unsigned int> inverters;
	std::string output;
	if (use_sizing) {
		output += "==============\nPUN Network: \n" + pun->Mosfet("Vdd", "y", PUN, inverters, transistor_count, p * l, l);
		output += "==============\nPDN Network: \n" + pdn->Mosfet("y", "gnd", PDN, inverters, transistor_count, n * l, l);
	}
	else {
		output += "==============\nPUN Network: \n" + pun->Mosfet("Vdd", "y", PUN, inverters, transistor_count);
		output += "==============\nPDN Network: \n" + pdn->Mosfet("y", "gnd", PDN, inverters, transistor_count);
	}

	if (inverters.size() > 0) {
		std::string output2 = "Inverters:\n";
		if (use_sizing) {
			for (auto &inverter : inverters) {
				output2 += std::string("The inverter circuit for ") + inverter.first + " is used " + std::to_string(inverter.second) + " times: \n";
				output2 += std::string("\tMi") + std::to_string(transistor_count_inverters++) + " wire_inv_" + inverter.first + " " + inverter.first + " Vdd Vdd PMOS [W=" + std::to_string(p * l) + " L=" + std::to_string(l) + "]\n";
				output2 += std::string("\tMi") + std::to_string(transistor_count_inverters++) + " wire_inv_" + inverter.first + " " + inverter.first + " gnd gnd NMOS [W=" + std::to_string(n * l) + " L=" + std::to_string(l) + "]\n";
			}
		}
		else {
			for (auto &inverter : inverters) {
				output2 += std::string("The inverter circuit for ") + inverter.first + " is used " + std::to_string(inverter.second) + " times: \n";
				output2 += std::string("\tMi") + std::to_string(transistor_count_inverters++) + " wire_inv_" + inverter.first + " " + inverter.first + " Vdd Vdd PMOS\n";
				output2 += std::string("\tMi") + std::to_string(transistor_count_inverters++) + " wire_inv_" + inverter.first + " " + inverter.first + " gnd gnd NMOS\n";
			}
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
	float n = 0.0f;
	float p = 0.0f;
	float l = 0.0f;
	
	std::cout << "Boolean logic uses |, &, and ': \n";
	std::cout << "\tie: vo=vi&(b|c') p=4 L=2 n=2\n";
	std::cout << "\tNote, if any of p, L, or n are not defined, sizing will not be calculated.\n";
	std::cout << "\t\tAlso, only use spaces to seperate the fields! \n";
	std::cout << "\tPlease insert your input: \n";
	std::getline(std::cin, input);

    std::istringstream iss(input);
    std::string token;
    while (std::getline(iss, token, ' ')) {
		if (token[0] == 'l' || token[0] == 'L') {
			int found = token.find('=');
			if (found != std::string::npos) {
				std::string val = token.substr(found + 1);
				l = std::stof(val);
			}
		}
		else if (token[0] == 'n' || token[0] == 'N') {
			int found = token.find('=');
			if (found != std::string::npos) {
				std::string val = token.substr(found + 1);
				n = std::stof(val);
			}
		}
		else if (token[0] == 'p' || token[0] == 'P') {
			int found = token.find('=');
			if (found != std::string::npos) {
				std::string val = token.substr(found + 1);
				p = std::stof(val);
			}
		}
		else {
			int found = token.find('=');
			if (found != std::string::npos) {
				input = token.substr(found + 1);
			}
			else {
				input = token;
			}
		}
    }

    GraphConverter conv;
    Node *node_tree = conv.ConvertToNodes(input);
    std::cout << "Input: " << *node_tree << std::endl;
	bool use_sizing = (n != 0.0f) && (p != 0.0f) && (l != 0.0f);
    std::cout << ConvertToMofset(node_tree, p, n, l, use_sizing) << std::endl;
    delete node_tree;

#ifdef _MSC_VER
	system("pause");
#endif

    return 0;
}