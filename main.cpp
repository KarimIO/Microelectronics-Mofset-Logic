#include "graph.hpp"
#include "graph_converter.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <map>

void ConvertToMofset(Node *input, float p, float n, float l, bool use_sizing, std::string &output, std::string &file_output) {
	Node *pun = input->DeMorgan();
	std::cout << "PUN: " << *pun << std::endl;
	Node *pdn = pun->Invert();
	std::cout << "PDN: " << *pdn << std::endl;


	unsigned int transistor_count_inverters = 0;

	unsigned int transistor_count = 0;
	std::map<std::string, unsigned int> inverters;
	if (use_sizing) {
		std::string mosfet = pun->Mosfet("Vdd", "y", PUN, inverters, transistor_count, p * l, l);
		file_output += mosfet;
		output += "==============\nPUN Network: \n" + mosfet;

		mosfet = pdn->Mosfet("y", "gnd", PDN, inverters, transistor_count, n * l, l);
		file_output += mosfet;
		output += "==============\nPDN Network: \n" + mosfet;
	}
	else {
		std::string mosfet = pun->Mosfet("Vdd", "y", PUN, inverters, transistor_count);
		file_output += mosfet;
		output += "==============\nPDN Network: \n" + mosfet;

		mosfet = pdn->Mosfet("y", "gnd", PDN, inverters, transistor_count);
		file_output += mosfet;
		output += "==============\nPDN Network: \n" + mosfet;
	}

	if (inverters.size() > 0) {
		std::string output2 = "Inverters:\n", file_output_2;
		if (use_sizing) {
			for (auto &inverter : inverters) {
				std::string mosfet = std::string("\tMi") + std::to_string(transistor_count_inverters++) + " wire_inv_" + inverter.first + " " + inverter.first + " Vdd Vdd PMOS [W=" + std::to_string(p * l) + " L=" + std::to_string(l) + "]\n";
				mosfet += std::string("\tMi") + std::to_string(transistor_count_inverters++) + " wire_inv_" + inverter.first + " " + inverter.first + " gnd gnd NMOS [W=" + std::to_string(n * l) + " L=" + std::to_string(l) + "]\n";

				file_output_2 += mosfet;
				output2 += std::string("The inverter circuit for ") + inverter.first + " is used " + std::to_string(inverter.second) + " times: \n" + mosfet;
			}
		}
		else {
			for (auto &inverter : inverters) {
				std::string mosfet = std::string("\tMi") + std::to_string(transistor_count_inverters++) + " wire_inv_" + inverter.first + " " + inverter.first + " Vdd Vdd PMOS\n";
				mosfet += std::string("\tMi") + std::to_string(transistor_count_inverters++) + " wire_inv_" + inverter.first + " " + inverter.first + " gnd gnd NMOS\n";

				file_output_2 += mosfet;
				output2 += std::string("The inverter circuit for ") + inverter.first + " is used " + std::to_string(inverter.second) + " times: \n" + mosfet;
			}
		}

		file_output = file_output_2 + file_output;
		output = output2 + output;
	}

	output = output + "Total Number of transistors: " + std::to_string(transistor_count_inverters + transistor_count);

	delete pdn;
	delete pun;
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

	std::string output, file_output;
	ConvertToMofset(node_tree, p, n, l, use_sizing, output, file_output);
	delete node_tree;

    std::cout << output << std::endl;

	std::string path;
	std::cout << "Please input a path to write to. Type q to quit and prevent writing a file: ";
	std::cin >> path;

	if (path != "q") {
		std::ofstream file(path);

		file << file_output;

		file.close();

#ifdef _MSC_VER
		system("pause");
#endif
	}

    return 0;
}