#include <vector>
#include <memory>
#include "AssignNode.h"

using std::vector, std::shared_ptr;

AssignNode::AssignNode() {
  children = vector<shared_ptr<ASTNode>>{nullptr, nullptr};
}

void AssignNode::accept() {}

string AssignNode::toString() {
  return "Assign: " + children[0]->toString() +
  " | value: " + children[1]->toString();
}
