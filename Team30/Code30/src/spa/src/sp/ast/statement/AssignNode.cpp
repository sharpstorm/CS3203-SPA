#include "AssignNode.h"

#include <vector>
#include "sp/ast/ASTNode.h"

using std::vector;

AssignNode::AssignNode(LineNumber line) :
    StatementASTNode(ASTNODE_ASSIGN, "", line) {
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
void AssignNode::leave(IExtractor *e) {
  e->leaveAssign(this);
}
