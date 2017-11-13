#include "graph.hpp"
#include "graph_converter.hpp"
#include <iostream>
#include <string>

int main(int argc, const char *argv[]) {
    GraphConverter conv;
    Node *n = conv.ConvertToNodes("(a|c)&(b|c)'");
	InNode *a = new InNode('a');
	InNode *b = new InNode('b');
	InNode *c = new InNode('c');
	NotNode *c_ = new NotNode(c);
	AndNode *ab = new AndNode(a, b);
	OrNode *abc_ = new OrNode(ab, c_);
    std::cout << *abc_ << std::endl;
    std::cout << n->Traverse() << std::endl;
    delete n;

#ifdef _MSC_VER
	system("pause");
#endif

    return 0;
}