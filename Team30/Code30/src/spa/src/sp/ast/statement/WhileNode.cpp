#include "WhileNode.h"

#include "sp/ast/ASTNode.h"

WhileNode::WhileNode(LineNumber line) :
    StatementASTNode(ASTNODE_WHILE, "", line) {
  // index [0] = conditional expression
  // index [1] = statement list
  children.push_back(nullptr);
  children.push_back(nullptr);
}

void WhileNode::accept(IExtractor *e) const {
  e->visitWhile(this);
}

void WhileNode::leave(IExtractor *e) const {
  e->leaveWhile(this);
}
