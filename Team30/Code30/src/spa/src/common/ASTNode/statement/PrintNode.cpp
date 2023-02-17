#include <string>
#include <vector>
#include <memory>
#include "PrintNode.h"

using std::vector, std::shared_ptr, std::string;

PrintNode::PrintNode() {
  children = vector<shared_ptr<ASTNode>>{nullptr};
}

void PrintNode::accept(shared_ptr<Extractor> e) {
  e->visit(*this);
}

void PrintNode::leave(shared_ptr<Extractor> e) {}

string PrintNode::toString() {
  string ss = "Print: "
  + (children[0] == nullptr ? "none" : children[0]->toString());
  return ss;
}
