#include <memory>
#include "StatementListNode.h"

using std::shared_ptr;

StatementListNode::StatementListNode() :ASTNode(ASTNODE_STMTLST, "") {
  children = vector<shared_ptr<ASTNode>>{};
}

void StatementListNode::accept(shared_ptr<Extractor> e) {
  e->visit(*this);
}

void StatementListNode::leave(shared_ptr<Extractor> e) {
}

string StatementListNode::toString() {
  string ss = "StmtLst: \n";
  for (shared_ptr<ASTNode> node : children) {
    ss += node->toString() + "\n";
  }
  return ss;
}
