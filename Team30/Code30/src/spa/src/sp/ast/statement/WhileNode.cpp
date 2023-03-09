#include "WhileNode.h"

#include <vector>

#include "sp/ast/ASTNode.h"

using std::vector;

WhileNode::WhileNode(int lineNumber) :
    StatementASTNode(ASTNODE_WHILE, "", lineNumber) {
  // index [0] = conditional expression
  // index [1] = statement list
  children = vector<ASTNodePtr>{nullptr, nullptr};
}

void WhileNode::accept(Extractor* e) {
  e->visitWhile(this);
}

void WhileNode::leave(Extractor* e) {
  e->leaveWhile(this);
}

string WhileNode::toString() {
  string ss = ":while";
  ss += children[0] == nullptr ? "none" : children[0]->toString() + "\n";
  ss += "StmtLst\n";
  ss += children[1] == nullptr ? "none" : children[1]->toString() + "\n";
  return ss;
}
