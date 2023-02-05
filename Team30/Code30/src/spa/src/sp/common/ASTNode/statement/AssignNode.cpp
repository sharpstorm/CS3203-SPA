#include <vector>
#include <memory>
#include <string>
#include "AssignNode.h"

using std::vector, std::shared_ptr, std::string;

AssignNode::AssignNode() {
  // index [0] = assignee
  // index [1] = expression
  children = vector<shared_ptr<ASTNode>>{nullptr, nullptr};
}

void AssignNode::accept() {}

string AssignNode::toString() {
  return std::to_string(lineNumber) + ": Assign:   "
  + (children[0] == nullptr ? "none" : children[0]->toString())
  + " = "
  + (children[1] == nullptr ? "none" : children[1]->toString());
}
