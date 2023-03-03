#include "IfNode.h"

#include <vector>
#include "common/ast/ASTNode.h"

using std::vector;

IfNode::IfNode(int lineNumber) : StatementASTNode(ASTNODE_IF, "", lineNumber) {
  // index [0] = conditional expression
  // index [1] = Then StmtLst
  // index [2] = Else StmtLst
  children = vector<ASTNodePtr>{nullptr, nullptr, nullptr};
}

void IfNode::accept(Extractor* e) {
  e->visit(this);
}

void IfNode::leave(Extractor* e) {
  e->leave(this);
}

string IfNode::toString() {
  return ":if";
}
