#include "IfNode.h"

#include "sp/ast/ASTNode.h"

IfNode::IfNode(LineNumber line) :
    StatementASTNode(ASTNODE_IF, "", line) {
  // index [0] = conditional expression
  // index [1] = Then StmtLst
  // index [2] = Else StmtLst
  children.push_back(nullptr);
  children.push_back(nullptr);
  children.push_back(nullptr);
}

void IfNode::accept(IExtractor *e) const {
  e->visitIf(this);
}

void IfNode::leave(IExtractor *e) const {
  e->leaveIf(this);
}
