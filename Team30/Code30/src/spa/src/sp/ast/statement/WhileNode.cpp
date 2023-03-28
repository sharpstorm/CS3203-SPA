#include "WhileNode.h"

#include <vector>

#include "sp/ast/ASTNode.h"

using std::vector;

WhileNode::WhileNode(LineNumber line) :
    StatementASTNode(ASTNODE_WHILE, "", line) {
  // index [0] = conditional expression
  // index [1] = statement list
  children.push_back(nullptr);
  children.push_back(nullptr);
}

void WhileNode::accept(IExtractor* e) {
  e->visitWhile(this);
}

void WhileNode::leave(IExtractor* e) {
  e->leaveWhile(this);
}

string WhileNode::toString() {
  string ss = ":while";
  ss += children[0] == nullptr ? "none" : children[0]->toString() + "\n";
  ss += "StmtLst\n";
  ss += children[1] == nullptr ? "none" : children[1]->toString() + "\n";
  return ss;
}
