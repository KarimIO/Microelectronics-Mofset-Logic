#include "graph.hpp"
#include "graph_converter.hpp"
#include <iostream>

int main(int argc, const char *argv[]) {
    GraphConverter c;
    Node *n = c.ConvertToNodes("(a|c)&(b|c)'");
    std::cout << *n << std::endl;
    std::cout << n->Traverse() << std::endl;
    delete n;

    return 0;
}