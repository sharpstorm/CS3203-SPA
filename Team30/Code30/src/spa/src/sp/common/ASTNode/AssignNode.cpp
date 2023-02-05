#include <vector>
#include <memory>
#include <string>
#include "AssignNode.h"

using std::vector, std::shared_ptr, std::string;

AssignNode::AssignNode() {
  children = vector<shared_ptr<ASTNode>>{nullptr, nullptr};
}

void AssignNode::accept() {}

string AssignNode::toString() {
  return std::to_string(lineNumber) + ": Assign: " + children[0]->toString() +
  " | value: " + children[1]->toString();
}
