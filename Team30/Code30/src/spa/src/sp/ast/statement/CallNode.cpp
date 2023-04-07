#include "CallNode.h"

CallNode::CallNode(LineNumber line, const ProcedureName &pName) :
    StatementASTNode(ASTNODE_CALL, "", line), procName(pName) {
}

void CallNode::accept(IExtractor *e) const {
  e->visitCall(this);
}

ProcedureName CallNode::getName() const {
  return procName;
}
