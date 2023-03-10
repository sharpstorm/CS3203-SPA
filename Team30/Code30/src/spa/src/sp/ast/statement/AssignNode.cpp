#include "AssignNode.h"

#include <vector>
#include "sp/ast/ASTNode.h"

using std::vector;

AssignNode::AssignNode(int lineNumber) :
    StatementASTNode(ASTNODE_ASSIGN, "", lineNumber) {
  // index [0] = assignee
  // index [1] = expression
  children.push_back(nullptr);
  children.push_back(nullptr);
}

void AssignNode::accept(IExtractor* e) {
  e->visitAssign(this);
}

string AssignNode::toString() {
  return ":assign";
}
