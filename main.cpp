#include "graph.hpp"
#include <iostream>

// Not final. a&b|c'
Node *ConvertToNodes(std::string input) {
    InNode *a = new InNode('a');
    InNode *b = new InNode('b');
    InNode *c = new InNode('c');                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
    NotNode *c_ = new NotNode(c);
    AndNode *ab = new AndNode(a, b);
    OrNode *abc_ = new OrNode(ab, c_);

    return abc_;
}

int main(int argc, const char *argv[]) {
    Node *abc_ = ConvertToNodes("a&b|c'");
    std::cout << *abc_ << std::endl;
    std::cout << abc_->Traverse() << std::endl;
    delete abc_;

    return 0;
}