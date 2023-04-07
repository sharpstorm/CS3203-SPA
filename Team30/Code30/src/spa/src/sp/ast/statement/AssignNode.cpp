#include "AssignNode.h"

#include "sp/ast/ASTNode.h"

AssignNode::AssignNode(LineNumber line) :
    StatementASTNode(ASTNODE_ASSIGN, "", line) {
  // index [0] = assignee
  // index [1] = expression
  children.push_back(nullptr);
  children.push_back(nullptr);
}

void AssignNode::accept(IExtractor *e) const {
  e->visitAssign(this);
}

void AssignNode::leave(IExtractor *e) const {
  e->leaveAssign(this);
}
