#include <vector>
#include <memory>
#include <string>
#include "AssignNode.h"

using std::vector, std::shared_ptr, std::string;

AssignNode::AssignNode(int lineNumber) :
    StatementASTNode(ASTNODE_ASSIGN, "", lineNumber) {
  // index [0] = assignee
  // index [1] = expression
  children = vector<shared_ptr<ASTNode>>{nullptr, nullptr};
}

void AssignNode::accept(Extractor* e) {
  e->visit(this);
}

string AssignNode::toString() {
  return ":assign";
}
