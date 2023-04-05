#include "StatementListNode.h"

#include <vector>

using std::vector;

StatementListNode::StatementListNode() :ASTNode(ASTNODE_STMTLST) {
  children = vector<ASTNodePtr>{};
}

void StatementListNode::accept(IExtractor* e) {
  e->visitStmtList(this);
}

void StatementListNode::leave(IExtractor* e) {
  e->leaveStmtList(this);
}
