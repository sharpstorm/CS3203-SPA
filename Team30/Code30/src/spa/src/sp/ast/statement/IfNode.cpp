#include "IfNode.h"

#include <vector>
#include "sp/ast/ASTNode.h"

using std::vector;

IfNode::IfNode(int lineNumber) : StatementASTNode(ASTNODE_IF, "", lineNumber) {
  // index [0] = conditional expression
  // index [1] = Then StmtLst
  // index [2] = Else StmtLst
  children.push_back(nullptr);
  children.push_back(nullptr);
  children.push_back(nullptr);
}

void IfNode::accept(IExtractor* e) {
  e->visitIf(this);
}

void IfNode::leave(IExtractor* e) {
  e->leaveIf(this);
}

string IfNode::toString() {
  return ":if";
}
