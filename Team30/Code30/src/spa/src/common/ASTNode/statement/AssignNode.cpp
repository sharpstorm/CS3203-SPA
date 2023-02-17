#include <vector>
#include <memory>
#include <string>
#include "AssignNode.h"

using std::vector, std::shared_ptr, std::string;

AssignNode::AssignNode() {
  // index [0] = assignee
  // index [1] = expression
  children = vector<shared_ptr<ASTNode>>{nullptr, nullptr};
  type = ASTNODE_ASSIGN;
}

void AssignNode::accept(shared_ptr<Extractor> e) {
  e->visit(*this);
}

string AssignNode::toString() {
  return ":assign";
}
