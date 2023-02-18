#include "StatementListNode.h"

StatementListNode::StatementListNode() :ASTNode(ASTNODE_STMTLST) {
  children = vector<ASTNodePtr>{};
}

void StatementListNode::accept(Extractor* e) {
  e->visit(this);
}

string StatementListNode::toString() {
  string ss = "StmtLst: \n";
  for (ASTNodePtr node : children) {
    ss += node->toString() + "\n";
  }
  return ss;
}
