#include <string>
#include <vector>
#include <memory>
#include "PrintNode.h"

using std::vector, std::shared_ptr, std::string;

PrintNode::PrintNode() {
  children = vector<shared_ptr<ASTNode>>{nullptr};
}

void PrintNode::accept() {
}

string PrintNode::toString() {
  string ss;
  ss += std::to_string(lineNumber) + ": Print: "
  + (children[0] == nullptr ? "none" : children[0]->toString());
  return ss;
}
