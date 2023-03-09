#include "StatementListNode.h"

#include <vector>

using std::vector;

StatementListNode::StatementListNode() :ASTNode(ASTNODE_STMTLST) {
  children = vector<ASTNodePtr>{};
}

void StatementListNode::accept(Extractor* e) {
  e->visitStmtList(this);
}

void StatementListNode::leave(Extractor* e) {
  e->leaveStmtList(this);
}

string StatementListNode::toString() {
  string ss = "StmtLst: \n";
  for (ASTNodePtr node : children) {
    ss += node->toString() + "\n";
  }
  return ss;
}
