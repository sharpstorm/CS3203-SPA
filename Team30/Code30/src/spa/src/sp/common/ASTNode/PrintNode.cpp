#include <string>
#include <vector>
#include <memory>
#include "PrintNode.h"

using std::vector, std::shared_ptr, std::string;

PrintNode::PrintNode() {
  children = vector<shared_ptr<ASTNode>>{nullptr };
}

void PrintNode::accept() {
}

string PrintNode::toString() {
  return std::to_string(lineNumber) + ": Print: " + children[0]->toString();
}
