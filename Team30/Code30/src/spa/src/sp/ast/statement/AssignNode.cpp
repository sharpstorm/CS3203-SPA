#include "AssignNode.h"

#include <vector>
#include "common/ast/ASTNode.h"

using std::vector;

AssignNode::AssignNode(int lineNumber) :
    StatementASTNode(ASTNODE_ASSIGN, "", lineNumber) {
  // index [0] = assignee
  // index [1] = expression
  children = vector<ASTNodePtr>{nullptr, nullptr};
}

void AssignNode::accept(Extractor* e) {
  e->visit(this);
}

string AssignNode::toString() {
  return ":assign";
}
