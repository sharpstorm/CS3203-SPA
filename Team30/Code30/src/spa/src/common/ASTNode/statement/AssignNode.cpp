#include <vector>
#include <memory>
#include <string>
#include "AssignNode.h"

using std::vector, std::shared_ptr, std::string;

AssignNode::AssignNode() : StatementASTNode(ASTNODE_ASSIGN, "") {
  // index [0] = assignee
  // index [1] = expression
  children = vector<shared_ptr<ASTNode>>{nullptr, nullptr};
}

void AssignNode::accept(shared_ptr<Extractor> e) {
  e->visit(*this);
}

void AssignNode::leave(shared_ptr<Extractor> e) {}

string AssignNode::toString() {
  return ":assign";
}
