#include <memory>
#include "StatementListNode.h"

using std::shared_ptr;

StatementListNode::StatementListNode() {
  children = vector<shared_ptr<ASTNode>>{};
  type = ASTNODE_STMTLST;
}

void StatementListNode::accept(shared_ptr<Extractor> e) {
  e->visit(*this);
}

string StatementListNode::toString() {
  string ss = "StmtLst: \n";
  for (shared_ptr<ASTNode> node : children) {
    ss += node->toString() + "\n";
  }
  return ss;
}
