#include <vector>
#include <memory>
#include "PrintNode.h"

using std::vector, std::shared_ptr;

PrintNode::PrintNode() {
  children = vector<shared_ptr<ASTNode>>{nullptr };
}

void PrintNode::accept() {
}

string PrintNode::toString() {
  return "Print: " + children[0]->toString();
}
